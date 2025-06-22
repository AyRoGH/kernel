# Makefile for kernel project with multi-architecture support
# Compatible with Linux distributions and macOS

# Detect operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    OS = macos
else ifeq ($(UNAME_S),Linux)
    OS = linux
else
    OS = unknown
    $(warning Unknown operating system: $(UNAME_S))
endif

# Architecture configuration (default: i386)
# Usage: make ARCH=i386 or make ARCH=x86_64
ARCH ?= i386

# Validate architecture
ifeq ($(ARCH),i386)
    TARGET_ARCH = i386
    ARCH_FLAGS = -m32
    ARCH_DEFINE = -D__i386__
    ASFLAGS = -f elf32
    LDFLAGS = -m elf_i386
    QEMU_SYSTEM = qemu-system-i386
else ifeq ($(ARCH),x86_64)
    TARGET_ARCH = x86_64
    ARCH_FLAGS = -m64
    ARCH_DEFINE = -D__x86_64__
    ASFLAGS = -f elf64
    LDFLAGS = -m elf_x86_64
    QEMU_SYSTEM = qemu-system-x86_64
else
    $(error Unsupported architecture: $(ARCH). Use i386 or x86_64)
endif

# Tool detection and distribution compatibility
CC = gcc
AS = nasm
LD = ld

# macOS-specific tool adjustments
ifeq ($(OS),macos)
    # On macOS, prefer Homebrew or MacPorts versions
    CC := $(shell which x86_64-elf-gcc 2>/dev/null || which gcc)
    LD := $(shell which x86_64-elf-ld 2>/dev/null || which ld)
    AS := $(shell which nasm 2>/dev/null)
    
    # macOS-specific linker flags
    ifeq ($(ARCH),i386)
        LDFLAGS = -m elf_i386 --oformat elf32-i386
    else
        LDFLAGS = -m elf_x86_64 --oformat elf64-x86-64
    endif
else
    # Linux-specific settings
    ifeq ($(ARCH),i386)
        LDFLAGS = -m elf_i386
    else
        LDFLAGS = -m elf_x86_64
    endif
endif

# Detect GRUB tools (different names across systems)
ifeq ($(OS),macos)
    GRUB_MKRESCUE := $(shell which grub-mkrescue 2>/dev/null)
    ifeq ($(GRUB_MKRESCUE),)
        $(error GRUB mkrescue tool not found. Install with: brew install grub)
    endif
else
    GRUB_MKRESCUE := $(shell which grub-mkrescue 2>/dev/null || which grub2-mkrescue 2>/dev/null)
    ifeq ($(GRUB_MKRESCUE),)
        $(error GRUB mkrescue tool not found. Install grub-common, grub2-tools, or grub2-common)
    endif
endif

# Detect QEMU (check if available)
QEMU_CHECK := $(shell which $(QEMU_SYSTEM) 2>/dev/null)
ifeq ($(QEMU_CHECK),)
    $(warning QEMU $(QEMU_SYSTEM) not found. Install qemu-system-x86 or qemu)
    QEMU_AVAILABLE = no
else
    QEMU_AVAILABLE = yes
endif

# Detect if we're on a 64-bit system for i386 builds
ifeq ($(ARCH),i386)
    ifeq ($(OS),macos)
        # On macOS, check for cross-compilation tools
        GCC_MULTILIB_CHECK := $(shell echo | $(CC) $(ARCH_FLAGS) -E - 2>/dev/null && echo "yes" || echo "no")
        ifeq ($(GCC_MULTILIB_CHECK),no)
            $(warning 32-bit cross-compilation not available. Install x86_64-elf-gcc or i686-elf-gcc via brew)
        endif
    else
        GCC_MULTILIB_CHECK := $(shell echo | $(CC) $(ARCH_FLAGS) -E - 2>/dev/null && echo "yes" || echo "no")
        ifeq ($(GCC_MULTILIB_CHECK),no)
            $(warning 32-bit support not available. Install gcc-multilib or libc6-dev-i386)
        endif
    endif
endif

# Compiler flags
CFLAGS = $(ARCH_FLAGS) -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra $(ARCH_DEFINE)
ifeq ($(ARCH),x86_64)
    CFLAGS += -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
endif

# Directories
SRC_DIR = src
BOOT_DIR = boot/$(TARGET_ARCH)
INCLUDE_DIR = include
DRIVERS_DIR = drivers
BUILD_DIR = build/$(TARGET_ARCH)

# Files
BOOT_ASM_OBJS = $(BUILD_DIR)/boot.o
BOOT_OBJS = $(BUILD_DIR)/bootloader.o
KERNEL_OBJS = $(BUILD_DIR)/kernel.o
DRIVER_OBJS = $(BUILD_DIR)/vga.o
OBJS = $(BOOT_ASM_OBJS) $(BOOT_OBJS) $(KERNEL_OBJS) $(DRIVER_OBJS)

# Output
OUTPUT = $(BUILD_DIR)/kernel.bin

# Default target with dependency check
all: check-deps $(OUTPUT)

# Check dependencies
check-deps:
	@echo "Checking build dependencies for $(ARCH) architecture..."
	@which $(CC) >/dev/null 2>&1 || (echo "Error: gcc not found. Install build-essential or gcc"; exit 1)
	@which $(AS) >/dev/null 2>&1 || (echo "Error: nasm not found. Install nasm"; exit 1)
	@which $(LD) >/dev/null 2>&1 || (echo "Error: ld not found. Install binutils"; exit 1)
ifeq ($(ARCH),i386)
	@echo | $(CC) $(ARCH_FLAGS) -E - >/dev/null 2>&1 || (echo "Error: 32-bit compilation not supported. Install gcc-multilib (Ubuntu/Debian) or lib32-gcc-libs (Arch)"; exit 1)
endif
	@echo "Dependencies check passed for $(ARCH)"

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.o: $(BOOT_DIR)/boot.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/bootloader.o: $(SRC_DIR)/boot/bootloader.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(KERNEL_OBJS): $(SRC_DIR)/kernel.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/vga.o: $(DRIVERS_DIR)/display/vga.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OUTPUT): $(OBJS)
	$(LD) $(LDFLAGS) -T $(BOOT_DIR)/linker.ld -o $@ $(OBJS)

iso: check-grub $(OUTPUT)
	@echo "Creating bootable ISO for $(ARCH)..."
	mkdir -p $(BUILD_DIR)/isodir/boot/grub
	cp $(OUTPUT) $(BUILD_DIR)/isodir/boot/
	cp boot/grub.cfg $(BUILD_DIR)/isodir/boot/grub/
	$(GRUB_MKRESCUE) -o $(BUILD_DIR)/kernel.iso $(BUILD_DIR)/isodir
	@echo "ISO created: $(BUILD_DIR)/kernel.iso"

check-grub:
	@if [ -z "$(GRUB_MKRESCUE)" ]; then \
		echo "Error: GRUB mkrescue tool not found."; \
		echo "Install one of the following packages:"; \
		echo "  Ubuntu/Debian: grub-common grub-pc-bin"; \
		echo "  Fedora/RHEL:   grub2-tools grub2-pc-modules"; \
		echo "  Arch Linux:    grub"; \
		echo "  openSUSE:      grub2"; \
		exit 1; \
	fi

run: check-qemu iso
	@echo "Starting QEMU for $(ARCH)..."
	$(QEMU_SYSTEM) -cdrom $(BUILD_DIR)/kernel.iso

check-qemu:
	@if [ "$(QEMU_AVAILABLE)" = "no" ]; then \
		echo "Error: QEMU $(QEMU_SYSTEM) not found."; \
		echo "Install one of the following packages:"; \
		echo "  Ubuntu/Debian: qemu-system-x86"; \
		echo "  Fedora/RHEL:   qemu-system-x86"; \
		echo "  Arch Linux:    qemu"; \
		echo "  openSUSE:      qemu"; \
		exit 1; \
	fi

run-i386: 
	@$(MAKE) ARCH=i386 run

run-x86_64:
	@$(MAKE) ARCH=x86_64 run

# Install dependencies helper targets
install-deps-debian:
	@echo "Installing dependencies for Ubuntu/Debian..."
	sudo apt update
	sudo apt install gcc nasm binutils grub-common grub-pc-bin qemu-system-x86 xorriso

install-deps-fedora:
	@echo "Installing dependencies for Fedora/RHEL..."
	sudo dnf install gcc nasm binutils grub2-tools grub2-pc-modules qemu-system-x86 xorriso

install-deps-arch:
	@echo "Installing dependencies for Arch Linux..."
	sudo pacman -S gcc lib32-gcc-libs nasm binutils grub qemu xorriso

install-deps-opensuse:
	@echo "Installing dependencies for openSUSE..."
	sudo zypper install gcc gcc-32bit nasm binutils grub2 qemu xorriso

install-deps-macos:
	@echo "Installing dependencies for macOS..."
	@echo "Checking for package managers..."
	@if command -v brew >/dev/null 2>&1; then \
		echo "Using Homebrew..."; \
		brew install nasm grub qemu xorriso; \
		echo "Installing cross-compilation toolchain..."; \
		brew tap nativeos/i386-elf-toolchain; \
		brew install i386-elf-binutils i386-elf-gcc || \
		(echo "Installing x86_64-elf toolchain as fallback..."; \
		 brew install x86_64-elf-binutils x86_64-elf-gcc); \
	elif command -v port >/dev/null 2>&1; then \
		echo "Using MacPorts..."; \
		sudo port install nasm grub2 qemu xorriso; \
		echo "You may need to install cross-compilation tools manually."; \
	else \
		echo "No package manager found. Please install Homebrew or MacPorts first:"; \
		echo "Homebrew: /bin/bash -c \"\$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""; \
		echo "MacPorts: https://www.macports.org/install.php"; \
	fi

# Detect distribution/OS and install dependencies
install-deps:
	@if [ "$(OS)" = "macos" ]; then \
		$(MAKE) install-deps-macos; \
	elif [ -f /etc/debian_version ]; then \
		$(MAKE) install-deps-debian; \
	elif [ -f /etc/fedora-release ] || [ -f /etc/redhat-release ]; then \
		$(MAKE) install-deps-fedora; \
	elif [ -f /etc/arch-release ]; then \
		$(MAKE) install-deps-arch; \
	elif [ -f /etc/SuSE-release ] || [ -f /etc/SUSE-brand ]; then \
		$(MAKE) install-deps-opensuse; \
	else \
		echo "System not automatically detected. Please install manually:"; \
		echo "  gcc, nasm, binutils"; \
		echo "  grub tools, qemu-system-x86, xorriso"; \
	fi

clean:
	rm -rf build

clean-arch:
	rm -rf $(BUILD_DIR)

# Test target to verify both architectures build correctly
test: check-deps
	@echo "Testing kernel build for both architectures..."
	@echo "Building i386 kernel..."
	@$(MAKE) clean ARCH=i386
	@$(MAKE) ARCH=i386
	@echo "✓ i386 build successful"
	@echo ""
	@echo "Building x86_64 kernel..."
	@$(MAKE) clean ARCH=x86_64
	@$(MAKE) ARCH=x86_64
	@echo "✓ x86_64 build successful"
	@echo ""
	@echo "✓ All tests passed! Both architectures build successfully."

# Show system information
info:
	@echo "=== Build System Information ==="
	@echo "Operating System: $(OS) ($(UNAME_S))"
	@echo "Target Architecture: $(ARCH)"
	@echo "Compiler: $(CC) $(ARCH_FLAGS)"
	@echo "Assembler: $(AS) $(ASFLAGS)"
	@echo "Linker: $(LD) $(LDFLAGS)"
	@echo "GRUB Tool: $(GRUB_MKRESCUE)"
	@echo "QEMU: $(QEMU_SYSTEM) (Available: $(QEMU_AVAILABLE))"
	@echo "Build Directory: $(BUILD_DIR)"
	@echo ""
	@echo "=== System Information ==="
	@uname -a
	@echo ""
	@if [ "$(OS)" = "macos" ]; then \
		echo "=== macOS Version ==="; \
		sw_vers; \
	elif [ -f /etc/os-release ]; then \
		echo "=== Distribution Information ==="; \
		grep PRETTY_NAME /etc/os-release; \
	fi

help:
	@echo "=== Multi-Architecture Kernel Build System ==="
	@echo "Supports Linux distributions and macOS"
	@echo ""
	@echo "Current system: $(OS) ($(UNAME_S))"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build kernel for current architecture ($(ARCH))"
	@echo "  iso              - Create bootable ISO for current architecture"
	@echo "  run              - Build and run kernel with QEMU"
	@echo "  run-i386         - Build and run 32-bit kernel"
	@echo "  run-x86_64       - Build and run 64-bit kernel"
	@echo "  clean            - Clean all build files"
	@echo "  clean-arch       - Clean current architecture build files"
	@echo "  check-deps       - Check if all dependencies are installed"
	@echo "  install-deps     - Auto-detect system and install dependencies"
	@echo "  info             - Show build system and environment information"
	@echo "  help             - Show this help message"
	@echo ""
	@echo "Architecture selection:"
	@echo "  make ARCH=i386   - Build for 32-bit (default)"
	@echo "  make ARCH=x86_64 - Build for 64-bit"
	@echo ""
	@echo "Dependencies installation (manual):"
	@echo "  make install-deps-debian   - Install deps for Ubuntu/Debian"
	@echo "  make install-deps-fedora   - Install deps for Fedora/RHEL"
	@echo "  make install-deps-arch     - Install deps for Arch Linux"
	@echo "  make install-deps-opensuse - Install deps for openSUSE"
	@echo "  make install-deps-macos    - Install deps for macOS"
	@echo ""
	@echo "Examples:"
	@echo "  make                      - Build i386 kernel"
	@echo "  make ARCH=x86_64          - Build x86_64 kernel"
	@echo "  make run ARCH=x86_64      - Build and run x86_64 kernel"
	@echo "  make install-deps         - Auto-install dependencies"

.PHONY: all clean clean-arch run run-i386 run-x86_64 iso help check-deps check-grub check-qemu info test
.PHONY: install-deps install-deps-debian install-deps-fedora install-deps-arch install-deps-opensuse install-deps-macos
