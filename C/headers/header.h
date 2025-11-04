#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include <stdarg.h>
#include <stdbool.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define VGA_PORT_COMMAND 0x3D4
#define VGA_PORT_DATA    0x3D5

#define F1 59
#define F2 60
#define F3 61
#define F4 62

#define NUM_PROFILES 4

enum vga_color {
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

// utils.c
uint32_t ft_strlen(const char* str);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);

// kernel.c
void term_init();
void term_set_color(uint8_t color);
void term_put_entry_at(char c, uint8_t color, size_t x, size_t y);
void term_write(const char* str, size_t len);
void term_write_str(const char* str);
void term_move_cursor();
void term_put_char(char c);
void term_scroll();
void switch_profile(int profile);	

// printk.c
void ft_printk(const char* fmt, ...);

// input.c
char keyboard_getchar(void);

// print_message.c
void print_open_message();
void print_user();

// init.c
void    init_all_profil();