; Multiboot2 header constants
MB2_HEADER__MAGIC               equ 0xE85250D6
MB2_HEADER__ARCHITECTURE_I386   equ 0
MB2_HEADER__HEADER_LENGTH       equ mb2_header_tag__end - mb2_header
MB2_HEADER__CHECKSUM            equ -(MB2_HEADER__MAGIC + MB2_HEADER__ARCHITECTURE_I386 + MB2_HEADER__HEADER_LENGTH)

MB2_HEADER_TAG__END                     equ 0

STACK_SIZE equ 0x4000

; Paging constants
PAGE_PRESENT    equ 1
PAGE_WRITABLE   equ 2
PAGE_HUGE       equ (1 << 7)

section .bss
align 16
stack_bottom:
    resb STACK_SIZE
stack_top:

; Multiboot2 parameters storage
multiboot2_magic: resd 1
multiboot2_info: resd 1

section .paging nobits
alignb 4096
pml4:
    resb 4096  ; PML4 table (4096 bytes)
pdpt:
    resb 4096  ; Page Directory Pointer Table (4096 bytes)  
page_directory:
    resb 4096  ; Page Directory (4096 bytes)

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

    ; Save multiboot2 parameters for later use in 64-bit mode
    mov [multiboot2_magic], eax
    mov [multiboot2_info], ebx

    cli

    ; Check for CPUID support
    call check_cpuid
    test eax, eax
    jz .no_long_mode

    ; Check for long mode support
    call check_long_mode
    test eax, eax
    jz .no_long_mode

    ; Set up page tables
    call setup_page_tables

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Load page table
    mov eax, pml4
    mov cr3, eax

    ; Enable long mode
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64.pointer]

    ; Far jump to 64-bit code
    jmp gdt64.code:long_mode_start

.no_long_mode:
    ; Handle error - no long mode support
    hlt
    jmp .no_long_mode

check_cpuid:
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    xor eax, ecx
    jz .no_cpuid
    mov eax, 1
    ret
.no_cpuid:
    mov eax, 0
    ret

check_long_mode:
    ; test if extended processor info in available
    mov eax, 0x80000000    ; implicit argument for cpuid
    cpuid                  ; get highest supported argument
    cmp eax, 0x80000001    ; it needs to be at least 0x80000001
    jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
    mov eax, 0x80000001    ; argument for extended processor info
    cpuid                  ; returns various feature bits in ecx and edx
    test edx, 1 << 29      ; test if the LM-bit is set in the D-register
    jz .no_long_mode       ; If it's not set, there is no long mode
    mov eax, 1
    ret
.no_long_mode:
    mov eax, 0
    ret

setup_page_tables:
    ; Map first PML4 entry to PDPT
    mov eax, pdpt
    or eax, PAGE_PRESENT | PAGE_WRITABLE
    mov [pml4], eax

    ; Map first PDPT entry to page directory
    mov eax, page_directory
    or eax, PAGE_PRESENT | PAGE_WRITABLE
    mov [pdpt], eax

    ; Map each page directory entry to a 2MiB page
    mov ecx, 0

.map_page_directory_table:
    ; map ecx-th entry
    mov eax, 0x200000  ; 2MiB
    mul ecx            ; start address of ecx-th page
    or eax, PAGE_PRESENT | PAGE_WRITABLE | PAGE_HUGE
    mov [page_directory + ecx * 8], eax ; map ecx-th entry

    inc ecx            ; increase counter
    cmp ecx, 512       ; if counter == 512, the whole P2 table is mapped
    jne .map_page_directory_table  ; else map the next entry

    ret

section .rodata
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41) | (1<<43) | (1<<53) ; code segment
.data: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41) ; data segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

section .text
bits 64
long_mode_start:
    ; clear all data segment registers
    mov ax, gdt64.data
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up stack
    mov rsp, stack_top

    ; Load multiboot2 parameters for 64-bit calling convention
    ; In x86-64, first two parameters go in RDI and RSI
    mov edi, [multiboot2_magic]
    mov esi, [multiboot2_info]

    call kernel_main

loop:
    hlt
    jmp loop
