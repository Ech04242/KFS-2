#include "../headers/header.h"

extern uint8_t inb(uint16_t port);

static const char scancode_set1[128] = {
    0, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', //0 -> 14
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // !5 -> 28
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', // 29 -> 43
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, // 44 -> 54
    0, 0, ' ', 0, F1, F2, F3, F4}; // 55 -> 59

static const char scancode_set2[128] = {
    0, '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '`', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 
    0, 0, ' ', 0, F1, F2, F3, F4};


char keyboard_getchar(void) {
	static bool shift_pressed = false;
	uint8_t scancode = inb(0x60);
	if (scancode == 42){
		shift_pressed = true;
		return(0);
	}
	else if (scancode == 170){
		shift_pressed = false;
		return (0);
	}
	if (scancode & 0x80)
		return (0);
	if (shift_pressed == false)
		return (scancode_set1[scancode]);
	else
		return (scancode_set2[scancode]);
}