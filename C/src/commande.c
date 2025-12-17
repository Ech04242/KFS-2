#include "../headers/header.h"

extern uint16_t*	terminal_buffer;
extern void			outb(uint16_t port, uint8_t value);
extern size_t		terminal_row[NUM_PROFILES];
extern uint8_t		activ_user;


static void reboot() {
	ft_printk("\nSystem reboot\n");
    outb(0x64, 0xFE);
    while(1) { 
		__asm__ volatile ("hlt"); 
	}
}

void check_cmd(){
	char cmd[VGA_WIDTH - 6];

	size_t	start = 6 + (VGA_WIDTH * (terminal_row[activ_user]));
	size_t	end = start + 72;   
	
	for (size_t i = 0; start <= end; start++) {
		cmd[i++] = (char)terminal_buffer[start];
	}

	if (!(ft_memcmp(cmd, "reboot\0", 7))){
		reboot();
	}
}
