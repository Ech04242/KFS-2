ENTRY(_start)

SECTIONS
{
    . = 1M;
    .text ALIGN(4K) : {
        *(.multiboot)
        *(.text)
    }
    .data : { *(.data) }
    .bss : { *(.bss) }
}
