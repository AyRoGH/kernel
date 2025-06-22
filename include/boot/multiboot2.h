#ifndef __INCLUDE__BOOT__MULTIBOOT2_H__
#define __INCLUDE__BOOT__MULTIBOOT2_H__

#include <types.h>

/* HEADER */

#define MB2_HEADER__MAGIC 0xE85250D6

#define MB2_HEADER_TAG__END                     ((u16)0)
#define MB2_HEADER_TAG__INFO_REQUEST            ((u16)1)
#define MB2_HEADER_TAG__ADDRESS                 ((u16)2)
#define MB2_HEADER_TAG__ENTRY_ADDRESS           ((u16)3)
#define MB2_HEADER_TAG__CONSOLE_FLAGS           ((u16)4)
#define MB2_HEADER_TAG__FRAMEBUFFER             ((u16)5)
#define MB2_HEADER_TAG__MODULE_ALIGNMENT        ((u16)6)
#define MB2_HEADER_TAG__EFI_BOOT_SERVICE        ((u16)7)
#define MB2_HEADER_TAG__ENTRY_ADDRESS_EFI_I386  ((u16)8)
#define MB2_HEADER_TAG__ENTRY_ADDRESS_EFI_AMD64 ((u16)9)
#define MB2_HEADER_TAG__RELOCATABLE_HEADER      ((u16)10)

struct mb2_header__tag
{
    u16 type;
    u16 flags;
    u32 size;
} __attribute__ ((__packed__));

struct mb2_header
{
    u32 magic;
    u32 architecture;
    u32 header_length;
    u32 checksum;
    struct mb2_header__tag tags[];
} __attribute__ ((__packed__));

struct mb2_header_tag__info_request
{
    u16 type;   /* = 1 */
    u16 flags;
    u32 size;
    u32 mbi_tag_type[];
} __attribute__ ((__packed__));

struct mb2_header_tag__address
{
    u16 type;   /* = 2 */
    u16 flags;
    u32 size;
    u32 header_addr;
    u32 load_addr;
    u32 load_end_addr;
    u32 bss_end_addr;
} __attribute__ ((__packed__));

struct mb2_header_tag__entry_address
{
    u16 type;   /* = 3 */
    u16 flags;
    u32 size;
    u32 entry_addr;
} __attribute__ ((__packed__));

struct mb2_header_tag__console_flags
{
    u16 type;   /* = 4 */
    u16 flags;
    u32 size;   /* = 12 */
    u32 console_flags;
} __attribute__ ((__packed__));

struct mb2_header_tag__framebuffer
{
    u16 type;   /* = 5 */
    u16 flags;
    u32 size;   /* = 20 */
    u32 width;
    u32 height;
    u32 depth;
} __attribute__ ((__packed__));

struct mb2_header_tag__module_alignment
{
    u16 type;   /* = 6 */
    u16 flags;
    u32 size;   /* = 8 */
} __attribute__ ((__packed__));

struct mb2_header_tag__efi_boot_service
{
    u16 type;   /* = 7 */
    u16 flags;
    u32 size;   /* = 8 */
} __attribute__ ((__packed__));

struct mb2_header_tag__entry_address_efi_i386
{
    u16 type;   /* = 8 */
    u16 flags;
    u32 size;
    u32 entry_addr;
} __attribute__ ((__packed__));

struct mb2_header_tag__entry_address_efi_amd64
{
    u16 type;   /* = 9 */
    u16 flags;
    u32 size;
    u32 entry_addr;
} __attribute__ ((__packed__));

struct mb2_header_tag__relocatable_header
{
    u16 type;   /* = 10 */
    u16 flags;
    u32 size;   /* = 24 */
    u32 min_addr;
    u32 max_addr;
    u32 align;
} __attribute__ ((__packed__));

/* BOOT INFORMATION */

#define MB2_INFO__MAGIC ((u64)0x36D76289)

#define MB2_INFO_TAG__TYPE__END                     ((u32)0)
#define MB2_INFO_TAG__TYPE__COMMAND_LINE            ((u32)1)
#define MB2_INFO_TAG__TYPE__BOOT_LOADER_NAME        ((u32)2)
#define MB2_INFO_TAG__TYPE__MODULE                  ((u32)3)
#define MB2_INFO_TAG__TYPE__BASIC_MEMORY            ((u32)4)
#define MB2_INFO_TAG__TYPE__BOOT_DEVICE             ((u32)5)
#define MB2_INFO_TAG__TYPE__MEMORY_MAP              ((u32)6)
#define MB2_INFO_TAG__TYPE__VBE                     ((u32)7)
#define MB2_INFO_TAG__TYPE__FRAMEBUFFER             ((u32)8)
#define MB2_INFO_TAG__TYPE__ELF_SECTIONS            ((u32)9)
#define MB2_INFO_TAG__TYPE__APM_TABLE               ((u32)10)
#define MB2_INFO_TAG__TYPE__EFI_32BIT_SYSTEM_TABLE  ((u32)11)
#define MB2_INFO_TAG__TYPE__EFI_64BIT_SYSTEM_TABLE  ((u32)12)
#define MB2_INFO_TAG__TYPE__SMBIOS                  ((u32)13)
#define MB2_INFO_TAG__TYPE__ACPI_OLD                ((u32)14)
#define MB2_INFO_TAG__TYPE__ACPI_NEW                ((u32)15)
#define MB2_INFO_TAG__TYPE__NETWORK                 ((u32)16)
#define MB2_INFO_TAG__TYPE__EFI_MEMORY_MAP          ((u32)17)
#define MB2_INFO_TAG__TYPE__EFI_BOOT_SERVICE        ((u32)18)
#define MB2_INFO_TAG__TYPE__EFI_32BIT_IMAGE_HANDLER ((u32)19)
#define MB2_INFO_TAG__TYPE__EFI_64BIT_IMAGE_HANDLER ((u32)20)
#define MB2_INFO_TAG__TYPE__LOAD_BASE_ADDR          ((u32)21)

#define MB2_INFO_TAG__MEMORY_MAP__ENTRY__TYPE__MEMORY_AVAILABLE         ((u32)1)
#define MB2_INFO_TAG__MEMORY_MAP__ENTRY__TYPE__MEMORY_RESERVED          ((u32)2)
#define MB2_INFO_TAG__MEMORY_MAP__ENTRY__TYPE__MEMORY_ACPI_RECLAIMABLE  ((u32)3)
#define MB2_INFO_TAG__MEMORY_MAP__ENTRY__TYPE__MEMORY_NVS               ((u32)4)
#define MB2_INFO_TAG__MEMORY_MAP__ENTRY__TYPE__MEMORY_BADRAM            ((u32)5)

#define MB2_INFO_TAG__FRAMEBUFFER__TYPE__INDEXED    ((u8)0)
#define MB2_INFO_TAG__FRAMEBUFFER__TYPE__DIRECT_RGB ((u8)1)
#define MB2_INFO_TAG__FRAMEBUFFER__TYPE__EGA_TEXT   ((u8)2)

struct mb2_info_tag
{
    u32 type;
    u32 size;
} __attribute__ ((__packed__));

struct mb2_info_tag__boot_command_line
{
    u32 type;   /* = 1 */
    u32 size;
    u32 string[0];
} __attribute__ ((__packed__));

struct mb2_info_tag__boot_loader_name
{
    u32 type;   /* = 2 */
    u32 size;
    char string[0];
} __attribute__ ((__packed__));

struct mb2_info_tag__module
{
    u32 type;   /* = 3 */
    u32 size;
    u32 mod_start;
    u32 mod_end;
    u32 string[0];
} __attribute__ ((__packed__));

struct mb2_info_tag__basic_memory
{
    u32 type;   /* = 4 */
    u32 size;   /* = 16 */
    u32 mem_lower;
    u32 mem_upper;
} __attribute__ ((__packed__));

struct mb2_info_tag__boot_device
{
    u32 type;   /* = 5 */
    u32 size;   /* = 20 */
    u32 biosdev;
    u32 partition;
    u32 sub_partition;
} __attribute__ ((__packed__));

struct mb2_info_tag__memory_map__entry
{
    u64 base_addr;
    u64 length;
    u32 type;
    u32 reserved;
} __attribute__ ((__packed__));

struct mb2_info_tag__memory_map
{
    u32 type;   /* = 6 */
    u32 size;
    u32 entry_size;
    u32 entry_version;
    struct mb2_info_tag__memory_map__entry entries[];
} __attribute__ ((__packed__));

struct mb2_info_tag__vbe
{
    u32 type;   /* = 7 */
    u32 size;   /* = 784 */
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;
    u8 vbe_control_info[512];
    u8 vbe_mode_info[256];
} __attribute__ ((__packed__));

struct mb2_info_tag__framebuffer
{
    u32 type;   /* = 8 */
    u32 size;
    u64 framebuffer_addr;
    u32 pitch;
    u32 width;
    u32 height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u8 reserved;
    union
    {
        struct mb2_info_tag__framebuffer__palette
        {
            u32 framebuffer_palette_num_colors;
            struct
            {
                u8 red_value;
                u8 green_value;
                u8 blue_value;
            } framebuffer_colors[];
        } framebuffer_palette;
        struct mb2_info_tag__framebuffer__direct_rgb
        {
            u8 framebuffer_red_field_position;
            u8 framebuffer_red_mask_size;
            u8 framebuffer_green_field_position;
            u8 framebuffer_green_mask_size;
            u8 framebuffer_blue_field_position;
            u8 framebuffer_blue_mask_size;
        } framebuffer_rgb;
    };
} __attribute__ ((__packed__));

struct mb2_info_tag__elf_sections
{
    u32 type;   /* = 9 */
    u32 size;
    u16 num;
    u16 entsize;
    u16 shndx;
    char sections[];
} __attribute__ ((__packed__));

struct mb2_info_tag__apm_table
{
    u32 type;   /* = 10 */
    u32 size;   /* = 28 */
    u16 version;
    u16 cseg;
    u32 offset;
    u16 cseg_16;
    u16 dseg;
    u16 flags;
    u16 cseg_len;
    u16 cseg_16_len;
    u16 dseg_len;
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_32bit_system_table
{
    u32 type;   /* = 11 */
    u32 size;   /* = 12 */
    u32 pointer;
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_64bit_system_table
{
    u32 type;   /* = 12 */
    u32 size;   /* = 16 */
    u64 pointer;
} __attribute__ ((__packed__));

struct mb2_info_tag__smbios
{
    u32 type;   /* = 13 */
    u32 size;
    u8 major;
    u8 minor;
    u8 reserved[6];
    u8 tables[];
} __attribute__ ((__packed__));

struct mb2_info_tag__acpi_old
{
    u32 type;   /* = 14 */
    u32 size;
    u8 rsdp[];
} __attribute__ ((__packed__));

struct mb2_info_tag__acpi_new
{
    u32 type;   /* = 15 */
    u32 size;
    u8 rsdp[];
} __attribute__ ((__packed__));

struct mb2_info_tag__network
{
    u32 type;   /* = 16 */
    u32 size;
    u8 dhcpack[];
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_memory_map
{
    u32 type;   /* = 17 */
    u32 size;
    u32 descriptor_size;
    u32 descriptor_version;
    u8 efi_memory_map[];
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_boot_service
{
    u32 type;   /* = 18 */
    u32 size;   /* = 8 */
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_32bit_image_handler
{
    u32 type;   /* = 19 */
    u32 size;   /* = 12 */
    u32 pointer;
} __attribute__ ((__packed__));

struct mb2_info_tag__efi_64bit_image_handler
{
    u32 type;   /* = 20 */
    u32 size;   /* = 18 */
    u64 pointer;
} __attribute__ ((__packed__));

struct mb2_info_tag__load_base_addr
{
    u32 type;   /* = 21 */
    u32 size;   /* = 12 */
    u32 load_base_addr;
} __attribute__ ((__packed__));

#endif /* __INCLUDE__BOOT__MULTIBOOT2_H__ */