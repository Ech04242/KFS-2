#include "../headers/header.h"

extern size_t terminal_row[NUM_PROFILES];
extern size_t terminal_column[NUM_PROFILES];
extern uint16_t* terminal_buffer;
extern uint16_t profile_buffers[NUM_PROFILES][VGA_WIDTH * VGA_HEIGHT];

void    init_all_profil(){
    for (uint8_t i = 1; i < NUM_PROFILES; i++){
    	ft_memcpy(profile_buffers[i], terminal_buffer, VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));
		terminal_row[i] = terminal_row[0];
		terminal_column[i] = terminal_column[0];
	}
}