#include <types.h>
#include <drivers/display/vga.h>

typedef struct
{
    size_t width;
    size_t height;
    struct
    {
        u32 x;
        u32 y;
    } cursor;
    u8 color;
    u16 *buffer;
} vga_t;

static vga_t vga;

static inline u8 vga_entry_color(enum vga_color foreground, enum vga_color background) 
{
	return foreground | background << 4;
}

static inline u16 vga_entry(unsigned char c, u8 color) 
{
	return (u16)c | (u16)color << 8;
}

int vga_init(void)
{
    vga.width = VGA_WIDTH;
    vga.height = VGA_HEIGHT;
    vga.color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga.buffer = VGA_BUFFER;

    for (vga.cursor.y = 0U; vga.cursor.y < vga.height; vga.cursor.y++)
        for (vga.cursor.x = 0U; vga.cursor.x < vga.width; vga.cursor.x++)
            vga.buffer[vga.cursor.y * vga.width + vga.cursor.x] = vga_entry(' ', VGA_COLOR_BLACK);

    vga.cursor.x = 0U;
    vga.cursor.y = 0U;

    return 0;
}

static inline void vga_putentryat(char c, u8 color, size_t x, size_t y)
{
    vga.buffer[y * vga.width + x] = vga_entry(c, color);

    return;
}

static inline void vga_putchar(char c)
{
    vga_putentryat(c, vga.color, vga.cursor.x, vga.cursor.y);

    vga.cursor.x = vga.cursor.x < vga.width ? vga.cursor.x + 1U : 0U;
    vga.cursor.y = vga.cursor.x != 0 ? vga.cursor.y : (vga.cursor.y < vga.height ? vga.cursor.y + 1U : 0U);

    return;
}

int vga_write(const char *str, size_t str_length)
{
    size_t counter = 0;

    for (; counter <= str_length && str[counter] != '\0'; counter++)
        vga_putchar(str[counter]);

    return counter;
}