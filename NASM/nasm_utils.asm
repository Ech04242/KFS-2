; Fonction outb : void outb(uint16_t port, uint8_t val);
    global outb
outb:
    ; Paramètres sur la pile: [esp + 4] = port, [esp + 8] = valeur
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

; Fonction inb : uint8_t inb(uint16_t port);
    global inb
inb:
    mov dx, [esp + 4]
    in al, dx
    movzx eax, al
    ret