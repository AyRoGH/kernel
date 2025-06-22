#include <types.h>
#include <boot/bootloader.h>
#include <boot/multiboot2.h>
#include <drivers/display/vga.h>

#ifdef __x86_64__
    #include <arch/x86_64/arch_types.h>
#else
    #include <arch/i386/arch_types.h>
#endif

void kernel_main(u32 multiboot2_magic_number, uintptr_t multiboot2_info_addr)
{
    struct mb2_info_tag *mb2_info_tag = (struct mb2_info_tag *)multiboot2_info_addr;
    char str[] = "Hello, World!";

    vga_init();
    vga_write(str, 14);
    
    if (bootloader(multiboot2_magic_number, mb2_info_tag))
        return;

    while (1);

    return;
}