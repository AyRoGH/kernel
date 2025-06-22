#ifndef __INCLUDE__ARCH_X86_64_ARCH_TYPES_H__
#define __INCLUDE__ARCH_X86_64_ARCH_TYPES_H__

/* x86_64-specific type definitions and constants */

/* Register types for x86_64 */
typedef u64 reg64_t;
typedef u32 reg32_t;
typedef u16 reg16_t;
typedef u8  reg8_t;

/* Page size for x86_64 */
#define PAGE_SIZE       4096
#define PAGE_SHIFT      12
#define PAGE_MASK       (PAGE_SIZE - 1)

/* Large page size (2MB) */
#define LARGE_PAGE_SIZE     (2 * 1024 * 1024)
#define LARGE_PAGE_SHIFT    21
#define LARGE_PAGE_MASK     (LARGE_PAGE_SIZE - 1)

/* Huge page size (1GB) */
#define HUGE_PAGE_SIZE      (1024 * 1024 * 1024)
#define HUGE_PAGE_SHIFT     30
#define HUGE_PAGE_MASK      (HUGE_PAGE_SIZE - 1)

/* Architecture name */
#define ARCH_NAME       "x86_64"
#define ARCH_BITS       64

/* Maximum virtual address space */
#define MAX_VIRT_ADDR   0x0000FFFFFFFFFFFFUL
/* Maximum physical address space (architecture dependent, typically 52 bits) */
#define MAX_PHYS_ADDR   0x000FFFFFFFFFFFFFUL

#endif /* __INCLUDE__ARCH_X86_64_ARCH_TYPES_H__ */
