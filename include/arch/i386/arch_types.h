#ifndef __INCLUDE__ARCH_I386_ARCH_TYPES_H__
#define __INCLUDE__ARCH_I386_ARCH_TYPES_H__

/* i386-specific type definitions and constants */

/* Register types for i386 */
typedef u32 reg32_t;
typedef u16 reg16_t;
typedef u8  reg8_t;

/* Page size for i386 */
#define PAGE_SIZE       4096
#define PAGE_SHIFT      12
#define PAGE_MASK       (PAGE_SIZE - 1)

/* Architecture name */
#define ARCH_NAME       "i386"
#define ARCH_BITS       32

/* Maximum physical address space (4GB for i386) */
#define MAX_PHYS_ADDR   0xFFFFFFFFUL

#endif /* __INCLUDE__ARCH_I386_ARCH_TYPES_H__ */
