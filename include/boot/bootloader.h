#ifndef __INCLUDE__BOOT__BOOTLOADER_H__
#define __INCLUDE__BOOT__BOOTLOADER_H__

#include <types.h>
#include <boot/multiboot2.h>

int bootloader(u32 multiboot2_magic_number, struct mb2_info_tag *mb2_info_tag);

#endif /* __INCLUDE__BOOT__BOOTLOADER_H__ */