#include "../headers/header.h"

extern uint8_t inb(uint16_t port);
extern size_t terminal_row[NUM_PROFILES];
extern size_t terminal_column[NUM_PROFILES];
extern uint8_t terminal_color;
extern uint8_t	activ_user;


void kernel_main(void)
{
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
					term_put_entry_at(' ', 7, terminal_column[activ_user], terminal_row[activ_user]);
					term_move_cursor();
				}
			}
			else if (c >= F1 && c <= F4)
				switch_profile(c - F1);
			else if (c)
			{
				term_put_char(c);
        		if (c == '\n')
          		print_user();
			}
		}
	}
}