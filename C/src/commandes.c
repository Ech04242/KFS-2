#include "../headers/header.h"
#include "../headers/GDT_header.h"

extern uint16_t*	terminal_buffer;
extern size_t		terminal_row[NUM_PROFILES];
extern uint8_t		activ_user;
extern void			outb(uint16_t port, uint8_t value);
extern uint16_t		profile_buffers[NUM_PROFILES][VGA_WIDTH * VGA_HEIGHT];
extern size_t		terminal_column[NUM_PROFILES];
extern uint8_t		terminal_color;

extern gdt gdt_content[GDT_ENTRIES];

static void reboot() {
	ft_printk("\nSystem reboot\n");
    outb(0x64, 0xFE);
    while(1) { 
		__asm__ volatile ("hlt"); 
	}
}

static void	print_gdt(){
	uint32_t	base;
	uint32_t	limit;
	uint8_t		access;
	uint8_t		granularity;

	for (uint32_t i = 0; i < GDT_ENTRIES; ++i){
		base =
			(gdt_content[i].base_low) |
			(gdt_content[i].base_mid  << 16) |
			(gdt_content[i].base_high << 24);
		limit =
			(gdt_content[i].limit) |
			((gdt_content[i].flags & 0x0F) << 16);
		access		= gdt_content[i].access;
		granularity	= gdt_content[i].flags;
		ft_printk("GDT[%d]: base=%x limit=%x access=%x gran=%x\n",
				i, base, limit, access, granularity);
	}
}

static void clear(){
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry('\0', terminal_color);
		}
	}
	terminal_row[activ_user] = 0;
	terminal_column[activ_user] = 0;
	ft_memcpy(profile_buffers[activ_user], terminal_buffer, VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));
}

static void set_color(){
	size_t	color;
	size_t	pos;
	char	car;
	size_t	i;
	
	i = 0;
	pos = 6 + 6 + (VGA_WIDTH * (terminal_row[activ_user]));
	while ((char)terminal_buffer[pos] == ' ')
		pos ++;
	car	= (char)terminal_buffer[pos];
	color = 0;
	if ((char)terminal_buffer[pos + 3] != '\0'){
		ft_printk("\nCode couleur incorrect, trop de caracteres\n");
		return ;
	}
	if ((char)terminal_buffer[pos] == '\0'){
		ft_printk("\nCode couleur incorrect, pas de valeurs saisies\n");
		return ;
	}
	while (++i < 4 && car != '\0') {
		if (car < '0' || car > '9') {
			ft_printk("\nCode couleur incorrect, caractere non desiree\n");
			return ;
		}
		color = color * 10 + (car - '0');
		car	= (char)terminal_buffer[pos + i];
	}

	if (color > 255)
		ft_printk("\nValeur de la couleur trop elevee ( range : [0 - 255]\n");
	else {
		ft_printk("\n");
		terminal_color = color;	
	}
}

static void echo(){
	size_t	start = 6 + 5 + (VGA_WIDTH * (terminal_row[activ_user]));
	size_t	end = start + 67;

	char cmd[VGA_WIDTH - 6 - 5];
	for (size_t i = 0; start <= end; start++) {
		cmd[i++] = (char)terminal_buffer[start];
	}
	ft_printk("\n%s\n", cmd);
}

void check_cmd(){
	char cmd[VGA_WIDTH - 6];

	size_t	start = 6 + (VGA_WIDTH * (terminal_row[activ_user]));
	size_t	end = start + 72;
	
	for (size_t i = 0; start <= end; start++) {
		cmd[i++] = (char)terminal_buffer[start];
	}

	if (!(ft_memcmp(cmd, "reboot\0", 7)))
		reboot();
	else if(!(ft_memcmp(cmd, "gdt\0", 4)))
		print_gdt();
	else if(!(ft_memcmp(cmd, "help\0", 5)))
		print_help();
	else if(!(ft_memcmp(cmd, "clear\0", 6)))
		clear();
	else if(!(ft_memcmp(cmd, "color ", 6)))
		set_color();
	else if(!(ft_memcmp(cmd, "echo ", 5)))
		echo();
	else
		term_put_char('\n');
}