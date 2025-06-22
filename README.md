# 🏗️ **Multi-Architecture Support**: Build for both i386 and x86_64
- 🐧 **Multi-Platform Support**: Works on Linux distributions and macOS
- 🚀 **Multiboot2 Compatible**: Uses the Multiboot2 specification
- 🔧 **Modular Design**: Clean separation between architecture-specific and common code
- 📦 **Smart Build System**: Automatic tool detection and dependency management
- 🖥️ **QEMU Ready**: Automatic ISO generation and emulationitecture Kernel

A simple, educational operating system kernel supporting both 32-bit (i386) and 64-bit (x86_64) architectures.

## Features

- 🏗️ **Multi-Architecture Support**: Build for both i386 and x86_64
- 🐧🍎 **Multi-Distribution Support**: Works on Ubuntu, Debian, Fedora, Arch Linux, macOS, and more
- 🚀 **Multiboot2 Compatible**: Uses the Multiboot2 specification
- 🔧 **Modular Design**: Clean separation between architecture-specific and common code
- 📦 **Smart Build System**: Automatic tool detection and dependency management
- 🖥️ **QEMU Ready**: Automatic ISO generation and emulation
- 🎨 **VGA Display Driver**: Text-mode VGA driver with color support

## Quick Start

### Prerequisites

The build system automatically detects your operating system and can install dependencies for you:

```bash
# Automatic installation (detects your system)
make install-deps
```

Or install manually based on your system:

**macOS:**
```bash
# Using Homebrew (recommended)
brew install nasm grub qemu xorriso
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc

# Or using MacPorts
sudo port install nasm grub2 qemu xorriso
```

**Ubuntu/Debian:**
```bash
sudo apt install gcc nasm grub2-common grub-pc-bin qemu-system-x86 xorriso
```

**Fedora/RHEL:**
```bash
sudo dnf install gccnasm grub2-tools grub2-pc-modules qemu-system-x86 xorriso
```

**Arch Linux:**
```bash
sudo pacman -S gcc lib32-gcc-libs nasm grub qemu xorriso
```

**openSUSE:**
```bash
sudo zypper install gcc gcc-32bit nasm grub2 qemu xorriso
```

> 📋 For detailed platform-specific instructions, see [docs/DISTRIBUTIONS.md](docs/DISTRIBUTIONS.md)

### Checking Your Environment

Before building, you can check if everything is set up correctly:

```bash
# Check system information and tool availability
make info

# Verify all dependencies are installed
make check-deps

# Get help on available commands
make help
```

### Building and Running

#### For 32-bit (i386) - Default
```bash
# Build the kernel
make

# Build and run in QEMU
make run

# Or explicitly specify architecture
make ARCH=i386
make run ARCH=i386
```

#### For 64-bit (x86_64)
```bash
# Build the kernel
make ARCH=x86_64

# Build and run in QEMU
make run ARCH=x86_64
```

#### Cleaning
```bash
# Clean all builds
make clean

# Clean specific architecture
make clean ARCH=i386
make clean ARCH=x86_64
```

## Project Structure

```
kernel/
├── README.md          # This file
├── Makefile          # Multi-architecture build system
├── boot/             # Boot-related files
│   ├── grub.cfg      # GRUB configuration
│   ├── i386/         # 32-bit boot files
│   └── x86_64/       # 64-bit boot files
├── src/              # Source code
│   ├── kernel.c      # Main kernel entry point
│   └── boot/         # Boot-related C code
├── drivers/          # Hardware drivers
│   └── display/      # Display drivers
│       └── vga.c     # VGA text-mode driver
├── include/          # Header files
│   ├── types.h       # Architecture-adaptive type definitions
│   ├── arch/         # Architecture-specific headers
│   ├── boot/         # Boot-related headers
│   └── drivers/      # Driver headers
│       └── display/  # Display driver headers
│           └── vga.h # VGA driver interface
└── docs/             # Documentation
    ├── ARCHITECTURE.md   # Architecture details
    ├── DISTRIBUTIONS.md  # Platform-specific instructions
    └── DRIVERS.md        # Hardware drivers documentation
```

## Architecture Differences

### i386 (32-bit)
- **Memory Model**: 32-bit protected mode
- **Address Space**: 4 GB virtual address space
- **Registers**: 32-bit general-purpose registers
- **Complexity**: Simpler to understand and debug
- **Use Case**: Educational purposes, embedded systems

### x86_64 (64-bit)
- **Memory Model**: 64-bit long mode with automatic transition
- **Address Space**: Large virtual address space (up to 256 TB)
- **Registers**: 64-bit general-purpose registers + additional registers
- **Features**: 2MB large pages, extended instruction set
- **Use Case**: Modern systems, performance-critical applications

## Development

### Adding New Features

1. **Common Code**: Add to `src/` for architecture-independent features
2. **Architecture-Specific Code**: Add to `src/arch/ARCH/` (create if needed)
3. **Headers**: Use `include/` for common headers, `include/arch/ARCH/` for specific ones
4. **Drivers**: Add hardware drivers to `drivers/` with corresponding headers in `include/drivers/`

### Current Drivers

- **VGA Display Driver** (`drivers/display/vga.c`):
  - Text-mode VGA driver supporting 80x25 character display
  - 16-color support with configurable foreground/background
  - Character positioning and screen clearing
  - Functions: `vga_init()`, `vga_write()`, `vga_putchar()`

> 📋 For detailed driver documentation, see [docs/DRIVERS.md](docs/DRIVERS.md)

### Adding New Architecture

1. Create `boot/ARCH/` with bootloader and linker script
2. Create `include/arch/ARCH/arch_types.h` with architecture constants
3. Update `Makefile` with new architecture flags
4. Update `include/types.h` if new types are needed

### Debugging

- Use QEMU's built-in debugger: `make run ARCH=i386` then `Ctrl+Alt+2` and type `info registers`
- For GDB debugging, add `-s -S` to QEMU flags in Makefile
- Check kernel logs and multiboot information in your bootloader code

## Technical Details

### Boot Process

1. **GRUB** loads the kernel using Multiboot2
2. **Bootloader** (assembly) sets up initial environment
3. **Kernel** (C) takes control and initializes the system

### Memory Layout

- **i386**: Linear mapping starting at 1MB (0x100000)
- **x86_64**: Higher-half kernel with identity mapping for first 2MB

### Build System

The Makefile automatically:
- Detects target architecture from `ARCH` variable
- Sets appropriate compiler flags (-m32/-m64)
- Selects correct assembler format (elf32/elf64)
- Chooses right linker target (elf_i386/elf_x86_64)
- Uses appropriate QEMU emulator

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on both architectures
5. Submit a pull request

## License

This project is open source but currently not under any licence.

---

**Happy Kernel Hacking! 🎉**
