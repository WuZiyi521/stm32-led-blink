typedef unsigned int u32;
extern u32 _sidata, _sdata, _edata, _sbss, _ebss;
extern int main(void);
void Reset_Handler(void);
void Default_Handler(void) { for(;;); }
__attribute__((section(".isr_vector"), used))
void (* const vectors[])(void) = {
  (void (*)(void))0x20005000u, Reset_Handler,
};
void Reset_Handler(void){
  u32 *s=&_sidata,*d=&_sdata;
  while(d<&_edata)*d++=*s++;
  for(d=&_sbss; d<&_ebss;) *d++=0;
  main();
  for(;;);
}
