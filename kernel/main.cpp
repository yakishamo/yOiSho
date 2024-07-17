extern "C"
int KernelMain(){
	while(1) 
		asm("hlt");
}
