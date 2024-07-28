void hlt() {
	while(1) asm("hlt");
}

extern "C"
int KernelMain(){
	hlt();
}
