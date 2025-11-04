#include "../headers/header.h"

size_t terminal_row[NUM_PROFILES] ;
size_t terminal_column[NUM_PROFILES] ;
uint8_t terminal_color = 0;

uint8_t	activ_user = 0;

uint16_t profile_buffers[NUM_PROFILES][VGA_WIDTH * VGA_HEIGHT];
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

extern void outb(uint16_t port, uint8_t value);

static inline uint8_t vga_entry_color(enum vga_color text_color, enum vga_color bg_color)
{ 
	return (text_color | bg_color << 4); 
}

void term_move_cursor()
{
    if (terminal_row[activ_user] >= VGA_HEIGHT || terminal_column[activ_user] >= VGA_WIDTH)
        return;
    uint16_t pos = terminal_row[activ_user] * VGA_WIDTH + terminal_column[activ_user];
    outb(VGA_PORT_COMMAND, 14);
    outb(VGA_PORT_DATA, (pos >> 8) & 0xFF);
    outb(VGA_PORT_COMMAND, 15);
    outb(VGA_PORT_DATA, pos & 0xFF);
}

static inline uint16_t vga_entry(unsigned char c, uint8_t color)
{ 
	return ((uint16_t) c | (uint16_t) color << 8); 
}

void term_init()
{
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	for (uint8_t i = 0; i < NUM_PROFILES; i++){
		terminal_row[i] = 0;
		terminal_column[i] = 0;
	}
}

void term_set_color(uint8_t color)
{
	terminal_color = color;
}

void term_put_entry_at(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;

	terminal_buffer[index] = vga_entry(c, color);
	uint16_t* vga = (uint16_t*)VGA_MEMORY;
    vga[index] = vga_entry(c, color);
}

void switch_profile(int profile) {
	if (profile < 0 || profile >= NUM_PROFILES)
        return;
    ft_memcpy(profile_buffers[activ_user], terminal_buffer, VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));
    activ_user = profile;
    terminal_buffer = profile_buffers[profile];	
	term_move_cursor();

    uint16_t* vga = (uint16_t*)VGA_MEMORY;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = terminal_buffer[i];
    }
}

void term_scroll()
{
	ft_memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, (VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16_t));
    size_t last_line_start = (VGA_HEIGHT - 1) * VGA_WIDTH;
    for (size_t i = 0; i < VGA_WIDTH; i++) {
        terminal_buffer[last_line_start + i] = vga_entry(' ', terminal_color);
    }
	uint16_t* vga = (uint16_t*)VGA_MEMORY;
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		vga[i] = terminal_buffer[i];
	}
	terminal_column[activ_user] = 0;
	terminal_row[activ_user] = 24;
	term_move_cursor();
}

void term_put_char(char c)
{
	if (c == '\n')
	{
		terminal_column[activ_user] = 0;
		terminal_row[activ_user]++;
	}
	else
	{
		term_put_entry_at(c, terminal_color, terminal_column[activ_user], terminal_row[activ_user]);
		++terminal_column[activ_user];
		if (terminal_column[activ_user] == VGA_WIDTH)
		{
			terminal_column[activ_user] = 0;
			terminal_row[activ_user]++;
		}
	}
	if (terminal_row[activ_user] == VGA_HEIGHT)
		term_scroll(); 
	term_move_cursor();
}

void term_write(const char* str, size_t len)
{
	for (size_t i = 0; i < len; i++)
		term_put_char(str[i]);
}

void term_write_str(const char* str)
{ 
	term_write(str, ft_strlen(str)); 
}