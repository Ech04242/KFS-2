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


gdtr DW 0 ; For limit storage
     DQ 0 ; For base storage


_start:
    cli                    ; Désactive interruptions
    lgdt [gdtr]
    mov eax, cr0
    or al, 1
    mov cr0, eax           ; Passage en mode protege
    mov esp, stack_top     ; Initialise la pile
    call kernel_main       ; Appel du noyau en C




.hang:
    hlt                    ; Stoppe le processeur en attendant interruption
    jmp .hang              ; Boucle infinie



setGdt:
   MOV   [gdtr], DI
   MOV   [gdtr+2], RSI
   LGDT  [gdtr]
   RET


reloadSegments:
   ; Reload CS register:
   PUSH 0x08                 ; Push code segment to stack, 0x08 is a stand-in for your code segment
   LEA RAX, [rel .reload_CS] ; Load address of .reload_CS into RAX
   PUSH RAX                  ; Push this value to the stack
   RETFQ                     ; Perform a far return, RETFQ or LRETQ depending on syntax

.reload_CS:
   ; Reload data segment registers
   MOV   AX, 0x10 ; 0x10 is a stand-in for your data segment
   MOV   DS, AX
   MOV   ES, AX
   MOV   FS, AX
   MOV   GS, AX
   MOV   SS, AX
   RET

