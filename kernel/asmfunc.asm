bits 64
section .text

global IoOut8
IoOut8:
	mov dx, di
	mov al, sil
	out dx, al
	ret

global IoIn8
IoIn8:
	mov dx, di
	in al, dx
	ret
