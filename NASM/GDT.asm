section .text
	global setGdt
	global reloadSegments
	global reload_CS
	global gdtr

gdtr DW 0 ; For limit storage
	 DD 0 ; For base storage

setGdt:
	mov   AX, [esp + 4]
	mov   [gdtr], AX
	mov   EAX, [ESP + 8]
	mov   [gdtr + 2], EAX
	lgdt  [gdtr]
	call reloadSegments
	ret


reloadSegments:
	; Reload CS register containing code selector:
	jmp   0x08:.reload_CS ; 0x08 is a stand-in for your code segment
	ret

.reload_CS:
	; Reload data segment registers:
	mov   AX, 0x10 ; 0x10 is a stand-in for your data segment
	mov   DS, AX
	mov   ES, AX
	mov   FS, AX
	mov   GS, AX
	mov   SS, AX
	ret
