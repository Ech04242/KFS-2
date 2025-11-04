#include "../headers/header.h"

extern void term_put_char(char c);

static void itoa(int value, char* buffer)
{
    char tmp[12];
    int i = 0, j, sign = value;
    if (value == 0) {
        buffer[0] = '0'; 
        buffer[1] = '\0'; 
        return;
    }
    if (value < 0)
        value = -value;
    while (value && i < 11)
    {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    }
    if (sign < 0)
        tmp[i++] = '-';
    for (j = 0; j < i; ++j)
        buffer[j] = tmp[i - j - 1];
    buffer[i] = 0;
}

static void utoa_hex(uint32_t value, char* buffer)
{
    const char* digits = "0123456789abcdef";    
    bool started = false;
    int i = 2;
    buffer[0] = '0';
    buffer[1] = 'x';
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint8_t nybble = (value >> shift) & 0xF;
        if (nybble || started || shift == 0) {
            buffer[i++] = digits[nybble];
            started = true;
        }
    }
    buffer[i] = 0;
}

void ft_printk(const char* fmt, ...)
{
    va_list args;
    char c;

    va_start(args, fmt);
    c = *fmt;
    while (*fmt++)
    {
        if (c != '%')
            term_put_char(c);
        else {
            if (!(*fmt))
            {
                term_put_char('%');
                return ;
            }
            c = *fmt++;
            if (c == 's')
                term_write_str(va_arg(args, const char*));
            else if (c == 'd')
            {
                int val = va_arg(args, int);
                char buf[12];
                itoa(val, buf);
                const char* s = buf;
                term_write_str(s);
            }
            else if (c == 'x')
            {
                unsigned int val = va_arg(args, unsigned int);
                char buf[12];
                utoa_hex(val, buf);
                const char* s = buf;
                term_write_str(s);
            }
            else if (c == 'c')
            {
                char cval = (char)va_arg(args, int);
                term_put_char(cval);
            }
            else {
                term_put_char('%');
                if (c)
                    term_put_char(c);
            }
        }
        c = *fmt;
    }
    va_end(args);
}
