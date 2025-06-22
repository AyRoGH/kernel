# Multi-Platform Support

This kernel build system has been designed to work across different operating systems with automatic tool detection and dependency management.

## Supported Platforms

✅ **Fully Tested**
- Fedora 41

🔄 **Should Work** (with manual dependency installation)
- macOS
- Debian
- Ubuntu
- openSUSE Leap/Tumbleweed
- CentOS Stream
- Linux Mint
- Pop!_OS
- Manjaro
- Arch Linux

## Quick Setup

### Automatic Installation
The Makefile can detect your distribution and install dependencies automatically:

```bash
make install-deps
```

### Manual Installation by Platform

#### macOS
```bash
# Using Homebrew (recommended)
brew install nasm grub qemu xorriso
brew tap nativeos/i386-elf-toolchain
brew install i386-elf-binutils i386-elf-gcc

# Using MacPorts (alternative)
sudo port install nasm grub2 qemu xorriso
# Note: You may need to install cross-compilation tools manually with MacPorts
```

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install gcc gcc-multilib nasm binutils grub-common grub-pc-bin qemu-system-x86 xorriso
```

#### Fedora/RHEL
```bash
sudo dnf install gcc gcc-multilib nasm binutils grub2-tools grub2-pc-modules qemu-system-x86 xorriso
```

#### Arch Linux
```bash
sudo pacman -S gcc lib32-gcc-libs nasm binutils grub qemu xorriso
```

#### openSUSE
```bash
sudo zypper install gcc gcc-32bit nasm binutils grub2 qemu xorriso
```

## Tool Detection

The build system automatically detects and adapts to:

### GRUB Tools
- `grub-mkrescue` (Ubuntu/Debian/Arch)
- `grub2-mkrescue` (Fedora/RHEL/openSUSE)

### QEMU Emulators  
- `qemu-system-i386` for 32-bit kernels
- `qemu-system-x86_64` for 64-bit kernels

### GCC Multilib Support
- Automatically checks for 32-bit compilation support when building i386
- Provides helpful error messages with distribution-specific package names

## Build System Features

### Dependency Checking
```bash
make check-deps           # Check all dependencies
make check-deps ARCH=i386 # Check i386-specific deps
```

### System Information
```bash
make info                 # Show build environment info
```

### Smart Error Messages
When dependencies are missing, the system provides:
- Clear error messages
- Distribution-specific package names
- Installation commands

## Common Issues and Solutions

### 32-bit Support Missing
**Error**: `32-bit compilation not supported`

**Solutions**:
- Ubuntu/Debian: `sudo apt install gcc-multilib libc6-dev-i386`
- Fedora: `sudo dnf install gcc-multilib glibc-devel.i686`
- Arch: `sudo pacman -S lib32-gcc-libs lib32-glibc`

### Cross-Compilation Tools Missing (macOS)
**Error**: `32-bit cross-compilation not available`

**Solutions**:
- Homebrew: `brew tap nativeos/i386-elf-toolchain && brew install i386-elf-gcc i386-elf-binutils`
- Alternative: `brew install x86_64-elf-gcc x86_64-elf-binutils` (for 64-bit cross-compilation)
- MacPorts: Install cross-compilation toolchain manually

### GRUB Tools Not Found
**Error**: `GRUB mkrescue tool not found`

**Solutions**:
- macOS: `brew install grub` (Homebrew) or `sudo port install grub2` (MacPorts)
- Ubuntu/Debian: `sudo apt install grub-common grub-pc-bin`
- Fedora: `sudo dnf install grub2-tools grub2-pc-modules`
- Arch: `sudo pacman -S grub`

### QEMU Not Available
**Error**: `QEMU not found`

**Solutions**:
- macOS: `brew install qemu` (Homebrew) or `sudo port install qemu` (MacPorts)
- Ubuntu/Debian: `sudo apt install qemu-system-x86`
- Fedora: `sudo dnf install qemu-system-x86`
- Arch: `sudo pacman -S qemu`

## Platform-Specific Notes

### macOS
- Uses Homebrew or MacPorts package managers
- Requires cross-compilation toolchain for kernel development
- GRUB tools: `grub-mkrescue` (same as Linux)
- Cross-compilation: `i386-elf-gcc` or `x86_64-elf-gcc`
- **Note**: macOS's default clang/gcc may not work for kernel development
- **Recommendation**: Install dedicated cross-compilation toolchain

#### macOS Setup Detailed Steps
1. Install Xcode Command Line Tools: `xcode-select --install`
2. Install Homebrew: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
3. Install dependencies: `make install-deps-macos`
4. Verify setup: `make info`

### Ubuntu/Debian
- Uses APT package manager
- GRUB tools: `grub-mkrescue`
- Multilib package: `gcc-multilib`

### Fedora/RHEL
- Uses DNF/YUM package manager  
- GRUB tools: `grub2-mkrescue`
- Multilib package: `gcc-multilib`

### Arch Linux
- Uses Pacman package manager
- GRUB tools: `grub-mkrescue`  
- Multilib: `lib32-gcc-libs`
- May need to enable multilib repository

### openSUSE
- Uses Zypper package manager
- GRUB tools: `grub2-mkrescue`
- Multilib: `gcc-32bit`

## Testing Your Environment

After installation, verify everything works:

```bash
# Check system info
make info

# Check dependencies
make check-deps

# Test build for both architectures
make clean
make ARCH=i386
make clean  
make ARCH=x86_64

# Test running (if QEMU available)
make run ARCH=i386
make run ARCH=x86_64
```

## Contributing

If you test this on a new distribution:
1. Document any required changes
2. Add distribution detection to the Makefile
3. Update this documentation
4. Submit a pull request

## Troubleshooting

### Getting Help
```bash
make help                 # Show all available targets
make info                 # Show current environment
```

### Common Debug Steps
1. Check `make info` output
2. Verify `make check-deps` passes
3. Try manual dependency installation
4. Check distribution-specific package names

### Reporting Issues
When reporting problems, include:
- Distribution name and version (`cat /etc/os-release`)
- Make info output (`make info`)
- Full error message
- Steps to reproduce
