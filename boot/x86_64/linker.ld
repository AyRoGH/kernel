ENTRY(start)

SECTIONS
{
    . = 1M;

    .multiboot2 :
    {
        *(.multiboot2)
    }

    .text : ALIGN(4K)
    {
        *(.text)
    }

    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }

    .data : ALIGN(4K)
    {
        *(.data)
    }

    .paging : ALIGN(4K)
    {
        *(.paging)
    }

    .bss : ALIGN(4K)
    {
        *(.bss)
    }
}
