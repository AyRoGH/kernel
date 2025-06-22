#ifndef __INCLUDE__DRIVERS__DISPLAY__VGA_H__
#define __INCLUDE__DRIVERS__DISPLAY__VGA_H__

#include <types.h>

#define VGA_WIDTH   ((size_t)80)
#define VGA_HEIGHT  ((size_t)25)
#define VGA_BUFFER  ((u16 *)0xB8000)

enum vga_color
{
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

int vga_init(void);
int vga_write(const char *str, size_t str_length);

#endif /* __INCLUDE__DRIVERS__DISPLAY__VGA_H__ */