section .text
	global setGdt
	global reloadSegments
	global reload_CS
	global gdtr

gdtr DW 0 ; For limit storage
	 DD 0 ; For base storage

setGdt:
	mov eax, [esp + 4]
	lgdt [eax]

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
  
	mov ax, 0x18
	mov gs, ax

	jmp 0x08:.flush

.flush:
	ret



reloadSegments:
	; Reload CS register containing code selector:
	jmp   0x10:.reload_CS ; 0x08 is a stand-in for your code segment
	ret

.reload_CS:
	; Reload data segment registers:
	mov   AX, 0x18 ; 0x10 is a stand-in for your data segment
	mov   DS, AX
	mov   ES, AX
	mov   FS, AX
	mov   GS, AX
	mov   SS, AX
	ret
