MBALIGN equ 1 << 0        ; Alignement des pages
MEMINFO equ 1 << 1        ; Permet d'avoir la carte mémoire
MBFLAGS equ MBALIGN | MEMINFO ; Gestion des flags multiboot
MAGIC equ 0x1BADB002      ; Magic number pour GRUB multiboot
CHECKSUM equ -(MAGIC + MBFLAGS)

; Header Multiboot, aligné 4 octets, placé au début du binaire
section .multiboot
    align 4
    dd MAGIC
    dd MBFLAGS
    dd CHECKSUM

section .bss
    align 16
stack_bottom:
    resb 16384            ; réserve 16 Ko pour la pile
stack_top:

section .text
    global _start
    extern kernel_main

_start:
    mov esp, stack_top     ; Initialise la pile
    call kernel_main       ; Appel du noyau en C
    cli                    ; Désactive interruptions

.hang:
    hlt                    ; Stoppe le processeur en attendant interruption
    jmp .hang              ; Boucle infinie

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
