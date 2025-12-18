// Used for creating GDT segment descriptors in 64-bit integer form.
 
#include "../headers/header.h"
#include "../headers/GDT_header.h"


gdt gdt_content[GDT_ENTRIES];
struct GDT_ptr *gdt_ptr = (struct GDT_ptr *) 0x00000800; // Information de osdev

void create_descriptor(uint32_t id, uint32_t base, uint32_t limit, uint8_t access, uint16_t flag)
{
	(void) flag;

	gdt_content[id].limit = limit & 0xFFFF;
	gdt_content[id].base_low = (base & 0xFFFF);
	gdt_content[id].base_mid = (base >> 16) & 0xFF;
	gdt_content[id].base_high = (base >> 16) & 0xFF;
	gdt_content[id].access = access;
    gdt_content[id].flags = ((limit >> 16) & 0x0F) | 0xC0;
}




int GDT_init(void)
{
	gdt_ptr->limit = (sizeof(gdt) * GDT_ENTRIES) - 1;
	gdt_ptr->base = (uint32_t) &gdt_content;

	create_descriptor(0, 0, 0, 0, 0);
	create_descriptor(1, 0, 0x000FFFFF, (uint8_t) (GDT_CODE_PL0), FLAG_32);
	create_descriptor(2, 0, 0x000FFFFF, (uint8_t) (GDT_DATA_PL0), FLAG_32);
	create_descriptor(3, 0, 0x000FFFFF, (uint8_t) (GDT_STACK_PL0), FLAG_32);
	create_descriptor(4, 0, 0x000FFFFF, (uint8_t) (GDT_CODE_PL3), FLAG_32);
	create_descriptor(5, 0, 0x000FFFFF, (uint8_t) (GDT_DATA_PL3), FLAG_32);
	create_descriptor(6, 0, 0x000FFFFF, (uint8_t) (GDT_STACK_PL3), FLAG_32);
	setGdt((uint32_t) gdt_ptr);
	return 0;
}