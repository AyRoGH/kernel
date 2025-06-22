; Multiboot2 header constants
MB2_HEADER__MAGIC               equ 0xE85250D6
MB2_HEADER__ARCHITECTURE_I386   equ 0
MB2_HEADER__HEADER_LENGTH       equ mb2_header_tag__end - mb2_header
MB2_HEADER__CHECKSUM            equ -(MB2_HEADER__MAGIC + MB2_HEADER__ARCHITECTURE_I386 + MB2_HEADER__HEADER_LENGTH)

MB2_HEADER_TAG__END                     equ 0

STACK_SIZE equ 0x4000

section .bss
align 16
stack_bottom:
    resb STACK_SIZE
stack_top:

section .multiboot2
align 8
mb2_header:
    dd MB2_HEADER__MAGIC
    dd MB2_HEADER__ARCHITECTURE_I386
    dd MB2_HEADER__HEADER_LENGTH
    dd MB2_HEADER__CHECKSUM
    .end:

mb2_header_tag__end:
    dw MB2_HEADER_TAG__END
    dw 1 ; flags
    dd mb2_header_tag__end.end - mb2_header_tag__end
    .end:

section .text
bits 32
global start
extern kernel_main

start:
    jmp mb2_entry

mb2_entry:
    mov esp, stack_top
    mov ebp, esp

    cli

    ; Push multiboot2 parameters (32-bit calling convention)
    push ebx  ; multiboot2 info pointer
    push eax  ; multiboot2 magic

    call kernel_main

    ; Should never reach here
    cli
loop:
    hlt
    jmp loop
