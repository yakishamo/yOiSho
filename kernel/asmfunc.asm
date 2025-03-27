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

; void SetDSAll(uint16_t value)
global SetDSAll
SetDSAll:
  mov ds, di
  mov es, di
  mov fs, di
  mov gs, di
  ret

; void SetCSSS(uint16_t cs, uint16_t ss)
global SetCSSS
SetCSSS:
  push rbp
  mov rbp, rsp
  mov ss, si
  mov rax, .next
  push rdi ;CS
  push rax ;RIP
  o64 retf
.next:
  mov rsp, rbp
  pop rbp
  ret

; uint64_t GetCS()
global GetCS
GetCS:
  mov rax, cs
  ret

; void LoadGDT(uint16_t limit, uint64_t offset)
global LoadGDT
LoadGDT:
  push rbp
  mov rbp, rsp
  sub rsp, 10
  mov [rsp] ,di ; limit
  mov [rsp+2], rsi ; offset
  lgdt [rsp]
  mov rsp, rbp
  pop rbp
  ret

; void SetCR3(uint64_t value)
global SetCR3
SetCR3:
  mov cr3, rdi
  ret

; void LoadIDT(uint16_t limit, uint64_t offset)
global LoadIDT
LoadIDT:
  push rbp
  mov rbp, rsp
  sub rsp, 10
  mov [rsp] ,di ; limit
  mov [rsp+2], rsi ; offset
  lidt [rsp]
  mov rsp, rbp
  pop rbp
  ret

; void DebugHlt(uint64_t value1, uint64_t value2, uint64_t value3)
global DebugHlt
DebugHlt:
  mov rax, rdi
  mov rbx, rsi
  mov rcx, rdx
.loop:
  hlt
  jmp .loop
