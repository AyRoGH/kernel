#include <types.h>
#include <boot/multiboot2.h>
#include <boot/bootloader.h>

static inline struct mb2_info_tag *command_line(struct mb2_info_tag__boot_command_line *tag);
static inline struct mb2_info_tag *boot_loader_name(struct mb2_info_tag__boot_loader_name *tag);
static inline struct mb2_info_tag *module(struct mb2_info_tag__module *tag);
static inline struct mb2_info_tag *basic_memory(struct mb2_info_tag__basic_memory *tag);
static inline struct mb2_info_tag *boot_device(struct mb2_info_tag__boot_device *tag);
static inline struct mb2_info_tag *memory_map(struct mb2_info_tag__memory_map *tag);
static inline struct mb2_info_tag *vbe(struct mb2_info_tag__vbe *tag);
static inline struct mb2_info_tag *framebuffer(struct mb2_info_tag__framebuffer *tag);
static inline struct mb2_info_tag *elf_sections(struct mb2_info_tag__elf_sections *tag);
static inline struct mb2_info_tag *apm_table(struct mb2_info_tag__apm_table *tag);
static inline struct mb2_info_tag *efi_32bit_system_table(struct mb2_info_tag__efi_32bit_system_table *tag);
static inline struct mb2_info_tag *efi_64bit_system_table(struct mb2_info_tag__efi_64bit_system_table *tag);
static inline struct mb2_info_tag *smbios(struct mb2_info_tag__smbios *tag);
static inline struct mb2_info_tag *acpi_old(struct mb2_info_tag__acpi_old *tag);
static inline struct mb2_info_tag *acpi_new(struct mb2_info_tag__acpi_new *tag);
static inline struct mb2_info_tag *network(struct mb2_info_tag__network *tag);
static inline struct mb2_info_tag *efi_memory_map(struct mb2_info_tag__efi_memory_map *tag);
static inline struct mb2_info_tag *efi_boot_service(struct mb2_info_tag__efi_boot_service *tag);
static inline struct mb2_info_tag *efi_32bit_image_handler(struct mb2_info_tag__efi_32bit_image_handler *tag);
static inline struct mb2_info_tag *efi_64bit_image_handler(struct mb2_info_tag__efi_64bit_image_handler *tag);
static inline struct mb2_info_tag *load_base_addr(struct mb2_info_tag__load_base_addr *tag);

int bootloader(u32 multiboot2_magic_number, struct mb2_info_tag *multboot2_info_tag)
{
    struct mb2_info_tag *tag;
    struct mb2_info_tag *next_tag;

    if (multiboot2_magic_number != MB2_INFO__MAGIC ||
        multboot2_info_tag == NULL)
        return -1;

    for (tag = multboot2_info_tag; tag->type != MB2_INFO_TAG__TYPE__END; tag = next_tag)
    {
        next_tag = NULL;
        
        switch (tag->type)
        {
            case MB2_INFO_TAG__TYPE__COMMAND_LINE:
                next_tag = command_line((struct mb2_info_tag__boot_command_line *)tag);
                break;
            case MB2_INFO_TAG__TYPE__BOOT_LOADER_NAME:
                next_tag = boot_loader_name((struct mb2_info_tag__boot_loader_name *)tag);
                break;
            case MB2_INFO_TAG__TYPE__MODULE:
                next_tag = module((struct mb2_info_tag__module *)tag);
                break;
            case MB2_INFO_TAG__TYPE__BASIC_MEMORY:
                next_tag = basic_memory((struct mb2_info_tag__basic_memory *)tag);
                break;
            case MB2_INFO_TAG__TYPE__BOOT_DEVICE:
                next_tag = boot_device((struct mb2_info_tag__boot_device *)tag);
                break;
            case MB2_INFO_TAG__TYPE__MEMORY_MAP:
                next_tag = memory_map((struct mb2_info_tag__memory_map *)tag);
                break;
            case MB2_INFO_TAG__TYPE__VBE:
                next_tag = vbe((struct mb2_info_tag__vbe *)tag);
                break;
            case MB2_INFO_TAG__TYPE__FRAMEBUFFER:
                next_tag = framebuffer((struct mb2_info_tag__framebuffer *)tag);
                break;
            case MB2_INFO_TAG__TYPE__ELF_SECTIONS:
                next_tag = elf_sections((struct mb2_info_tag__elf_sections *)tag);
                break;
            case MB2_INFO_TAG__TYPE__APM_TABLE:
                next_tag = apm_table((struct mb2_info_tag__apm_table *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_32BIT_SYSTEM_TABLE:
                next_tag = efi_32bit_system_table((struct mb2_info_tag__efi_32bit_system_table *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_64BIT_SYSTEM_TABLE:
                next_tag = efi_64bit_system_table((struct mb2_info_tag__efi_64bit_system_table *)tag);
                break;
            case MB2_INFO_TAG__TYPE__SMBIOS:
                next_tag = smbios((struct mb2_info_tag__smbios *)tag);
                break;
            case MB2_INFO_TAG__TYPE__ACPI_OLD:
                next_tag = acpi_old((struct mb2_info_tag__acpi_old *)tag);
                break;
            case MB2_INFO_TAG__TYPE__ACPI_NEW:
                next_tag = acpi_new((struct mb2_info_tag__acpi_new *)tag);
                break;
            case MB2_INFO_TAG__TYPE__NETWORK:
                next_tag = network((struct mb2_info_tag__network *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_MEMORY_MAP:
                next_tag = efi_memory_map((struct mb2_info_tag__efi_memory_map *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_BOOT_SERVICE:
                next_tag = efi_boot_service((struct mb2_info_tag__efi_boot_service *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_32BIT_IMAGE_HANDLER:
                next_tag = efi_32bit_image_handler((struct mb2_info_tag__efi_32bit_image_handler *)tag);
                break;
            case MB2_INFO_TAG__TYPE__EFI_64BIT_IMAGE_HANDLER:
                next_tag = efi_64bit_image_handler((struct mb2_info_tag__efi_64bit_image_handler *)tag);
                break;
            case MB2_INFO_TAG__TYPE__LOAD_BASE_ADDR:
                next_tag = load_base_addr((struct mb2_info_tag__load_base_addr *)tag);
                break;
            default:
                // Pour les tags inconnus, calculer next_tag manuellement
                next_tag = (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
                break;
        }
        
        // Vérification de sécurité pour éviter les pointeurs invalides
        if (next_tag == NULL || (u8*)next_tag < (u8*)tag) {
            return -1;
        }
    }

    return 0;
}

static inline struct mb2_info_tag *command_line(struct mb2_info_tag__boot_command_line *tag)
{
    /* TODO */

    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *boot_loader_name(struct mb2_info_tag__boot_loader_name *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *module(struct mb2_info_tag__module *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *basic_memory(struct mb2_info_tag__basic_memory *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *boot_device(struct mb2_info_tag__boot_device *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *memory_map(struct mb2_info_tag__memory_map *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *vbe(struct mb2_info_tag__vbe *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *framebuffer(struct mb2_info_tag__framebuffer *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *elf_sections(struct mb2_info_tag__elf_sections *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *apm_table(struct mb2_info_tag__apm_table *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_32bit_system_table(struct mb2_info_tag__efi_32bit_system_table *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_64bit_system_table(struct mb2_info_tag__efi_64bit_system_table *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *smbios(struct mb2_info_tag__smbios *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *acpi_old(struct mb2_info_tag__acpi_old *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *acpi_new(struct mb2_info_tag__acpi_new *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *network(struct mb2_info_tag__network *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_memory_map(struct mb2_info_tag__efi_memory_map *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_boot_service(struct mb2_info_tag__efi_boot_service *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_32bit_image_handler(struct mb2_info_tag__efi_32bit_image_handler *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *efi_64bit_image_handler(struct mb2_info_tag__efi_64bit_image_handler *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}

static inline struct mb2_info_tag *load_base_addr(struct mb2_info_tag__load_base_addr *tag)
{
    /* TODO */
    
    return (struct mb2_info_tag *)((u8 *)tag + ((tag->size + 7) & ~7));
}