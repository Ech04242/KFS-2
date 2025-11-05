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
    cli                    ; Désactive interruptions
    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax           ; Passage en mode protege
    jmp 08h:PModeMain
    mov esp, stack_top     ; Initialise la pile
    call kernel_main       ; Appel du noyau en C


PModeMain:
; load DS, ES, FS, GS, SS, ESP


.hang:
    hlt                    ; Stoppe le processeur en attendant interruption
    jmp .hang              ; Boucle infinie


