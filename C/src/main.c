#include "../headers/header.h"
#include "../headers/GDT_header.h"

extern uint8_t		inb(uint16_t port);
extern void			outb(uint16_t port, uint8_t value);
extern size_t		terminal_row[NUM_PROFILES];
extern size_t		terminal_column[NUM_PROFILES];
extern uint8_t		terminal_color;
extern uint8_t		activ_user;
extern uint16_t*	terminal_buffer;

extern gdt gdt_content[GDT_ENTRIES];


void reboot() {
    outb(0x64, 0xFE);
    while(1) { __asm__ volatile ("hlt"); }
}

static void gdt_print_entry(uint32_t i)
{
    uint32_t base =
        (gdt_content[i].base_low) |
        (gdt_content[i].base_mid  << 16) |
        (gdt_content[i].base_high << 24);
    uint32_t limit =
        (gdt_content[i].limit) |
        ((gdt_content[i].flags & 0x0F) << 16);
    uint8_t access      = gdt_content[i].access;
    uint8_t granularity = gdt_content[i].flags;
    ft_printk("GDT[%d]: base=%x limit=%x access=%x gran=%x\n",
            i, base, limit, access, granularity);
}


static void print_gdt(){
	for (uint32_t i = 0; i < GDT_ENTRIES; ++i)
        gdt_print_entry(i);
}

static void check_cmd(){
	char cmd[VGA_WIDTH - 6];

	size_t	start = 6 + (VGA_WIDTH * (terminal_row[activ_user]));
	size_t	end = start + 72;
	
	for (size_t i = 0; start <= end; start++) {
		cmd[i++] = (char)terminal_buffer[start];
	}

	if (!(ft_memcmp(cmd, "reboot\0", 7)))
		reboot();
	else if(!(ft_memcmp(cmd, "gdt\0", 4))){
		ft_printk("GDT OK !\n");
		print_gdt();
	}
}

void kernel_main(void)
{
	GDT_init();
	term_init();
	term_move_cursor();
	print_open_message();
	print_user();
	init_all_profil();
	while (42){
		uint8_t status = inb(0x64);
		if (status & 1)
		{
			char c = keyboard_getchar();
			if (c == '\b') {
				if (terminal_column[activ_user] > 6)
				{
					terminal_column[activ_user]--;
					term_put_entry_at('\0', 7, terminal_column[activ_user], terminal_row[activ_user]);
					term_move_cursor();
				}
			}
			else if (c >= F1 && c <= F4)
				switch_profile(c - F1);
			else if (c)
			{
				if (c == '\n'){
					check_cmd();
					term_put_char(c);
					print_user();
				}
				else {
					term_put_char(c);
				}
			}
		}
	}
}