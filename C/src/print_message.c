#include "../headers/header.h"

extern uint8_t terminal_color;

void print_open_message(){
	ft_printk(" ____ ____ ____ ____ ____ ____ \n");
	ft_printk("||K |||F |||S |||  |||4 |||2 ||\n");
	ft_printk("||__|||__|||__|||__|||__|||__||\n");
	ft_printk("|/__\\|/__\\|/__\\|/__\\|/__\\|/__\\|\n");
}

void	print_user(){
	uint8_t temp = terminal_color;

	terminal_color = VGA_COLOR_LIGHT_CYAN;
	ft_printk("USER >");
	terminal_color = temp;
}