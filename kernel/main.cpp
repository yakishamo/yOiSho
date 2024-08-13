void hlt() {
	while(1) asm("hlt");
}

extern "C"
// __attribute__((ms_abi))
int KernelMain(){
	hlt();
	return 0;
}
