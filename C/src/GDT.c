// Used for creating GDT segment descriptors in 64-bit integer form.
 
#include "../headers/header.h"
#include "../headers/GDT_header.h"


struct GDT gdt_ptr;

/*
void encodeGdtEntry(uint8_t *target, struct GDT source)
{
	// Check the limit to make sure that it can be encoded
//   if (source.limit > 0xFFFFF) {ft_printk("GDT cannot encode limits larger than 0xFFFFF");}
	
	// Encode the limit
	target[0] = source.limit & 0xFF;
	target[1] = (source.limit >> 8) & 0xFF;
	target[6] = (source.limit >> 16) & 0x0F;
	
	// Encode the base
	target[2] = source.base & 0xFF;
	target[3] = (source.base >> 8) & 0xFF;
	target[4] = (source.base >> 16) & 0xFF;
	target[7] = (source.base >> 24) & 0xFF;
	
	// Encode the access byte
	target[5] = source.access_byte;
	
	// Encode the flags
	target[6] |= (source.flags << 4);
}
*/

void create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
	uint64_t descriptor;
 
	// Create the high 16 bit segment
	descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
	descriptor |= (flag << 4) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
	descriptor |= (base >> 8) & 0x000000FF;         // set base bits 23:16
	descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
	// Shift by 16 to allow for low part of segment
	descriptor <<= 16;
 
	// Create the low 16 bit segment
	descriptor |= base  << 8;                       // set base bits 15:0
	descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
 
	ft_printk("0x%.16llX\n", descriptor);
}

int GDT_init(void)
{
	create_descriptor(0, 0, 0);
	create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
	create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
	create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
	create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));
	setGdt(gdt_ptr);
	return 0;
}