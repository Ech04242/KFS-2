#include "../headers/header.h"
#include "../headers/IDT_header.h"



__attribute__((unused))
static idtr_t idtr;

__attribute__((aligned(0x10), unused))
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance


void exception_handler()
{
	__asm__ volatile ("cli; hlt"); // Completely hangs the computer
}


