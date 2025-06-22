# Multi-Architecture Kernel Build System

This project currently supports two architectures: i386 (32-bit) and x86_64 (64-bit).

## Project Structure

```
kernel/
├── boot/
│   ├── grub.cfg       # Common GRUB configuration
│   ├── i386/          # i386-specific boot files
│   │   ├── boot.s     # i386 assembly bootloader
│   │   └── linker.ld  # i386 linker script
│   └── x86_64/        # x86_64-specific boot files
│       ├── boot.s     # x86_64 assembly bootloader with long mode transition
│       └── linker.ld  # x86_64 linker script
├── src/
│   ├── kernel.c       # Main kernel entry point
│   └── boot/
│       └── bootloader.c # Common bootloader functions
├── drivers/           # Hardware drivers
│   └── display/       # Display drivers
│       └── vga.c      # VGA text-mode driver with color support
├── include/
│   ├── types.h        # Generic types with automatic architecture selection
│   ├── arch/
│   │   ├── i386/
│   │   │   └── arch_types.h    # i386-specific types and constants
│   │   └── x86_64/
│   │       └── arch_types.h    # x86_64-specific types and constants
│   ├── boot/
│   │   ├── bootloader.h        # Bootloader headers
│   │   └── multiboot2.h        # Multiboot2 definitions
│   └── drivers/       # Driver headers
│       └── display/   # Display driver headers
│           └── vga.h  # VGA driver interface and color definitions
└── docs/
    └── ARCHITECTURE.md         # This documentation
```

## Compilation

### For i386 (32-bit) - default
```bash
make                    # Compile for i386
make ARCH=i386         # Explicitly for i386
make run               # Compile and run in i386
make run ARCH=i386     # Explicitly for i386
```

### For x86_64 (64-bit)
```bash
make ARCH=x86_64       # Compile for x86_64
make run ARCH=x86_64   # Compile and run in x86_64
```

### Cleaning
```bash
make clean             # Clean all builds
make clean ARCH=i386   # Clean only i386
make clean ARCH=x86_64 # Clean only x86_64
```

## Differences Between Architectures

### i386 (32-bit)
- 32-bit protected mode
- 4 GB address space
- 32-bit registers
- Simpler to debug

### x86_64 (64-bit)
- 64-bit long mode
- Automatic transition from protected mode to long mode
- Extended address space
- 64-bit registers
- 64-bit paging support with 2MB pages

## Environment Variables and Options

- `ARCH=i386`: Compile for 32-bit architecture
- `ARCH=x86_64`: Compile for 64-bit architecture

## Configuration Files

The system automatically detects the target architecture and:
- Uses the correct compilation flags (-m32 vs -m64)
- Selects the right assembler (elf32 vs elf64)
- Chooses the right linker (elf_i386 vs elf_x86_64)
- Uses the right QEMU emulator (i386 vs x86_64)
- Includes the right architecture headers

## Data Types

The `include/types.h` file automatically adapts to the architecture:
- Basic types (u8, u16, u32, s8, s16, s32) remain identical
- Architecture-dependent types (u64, s64, uintptr_t, size_t) adapt
- Specific headers (`arch_types.h`) provide architecture constants

## Adding a New Architecture

To add a new architecture:
1. Create `boot/ARCH/` with `boot.s` and `linker.ld`
2. Create `include/arch/ARCH/arch_types.h`
3. Add flags in the Makefile
4. Update `include/types.h` if necessary

## Hardware Drivers

### VGA Display Driver

The kernel includes a VGA text-mode driver located in `drivers/display/vga.c`:

**Features:**
- 80x25 character text mode
- 16-color palette support
- Cursor positioning
- Screen clearing and initialization
- String output with automatic cursor advancement

**API:**
- `int vga_init(void)`: Initialize VGA driver and clear screen
- `int vga_write(const char *str, size_t str_length)`: Write string to screen
- Color definitions available in `include/drivers/display/vga.h`

**Memory Layout:**
- VGA buffer located at physical address 0xB8000
- Each character entry consists of 2 bytes: character + color attribute
- Color attribute format: `[background:4][foreground:4]`

**Build Integration:**
The VGA driver is automatically compiled and linked with the kernel for both i386 and x86_64 architectures through the Makefile.
