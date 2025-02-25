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

global SwitchKernelStack
SwitchKernelStack:
	mov rsp, rdi
	add rsp, rsi
	call rdx
.fin:
	hlt
	jmp .fin

; CpuidGetVendor(char *str);
global CpuidGetVendor
CpuidGetVendor:
  push rbx ; rbx register must be preserved by callee.
  xor eax, eax
  cpuid
  mov [rdi], ebx
  mov [rdi + 0x4], edx
  mov [rdi + 0x8], ecx
  pop rbx
  ret
