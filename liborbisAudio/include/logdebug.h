#include <stdint.h>


uint64_t syscall2(uint64_t i_rdi, ...);
#define sys_log(args...) {\
      char logbuf[400];\
      snprintf(logbuf, 400, args);\
      syscall2(308, 2, logbuf); \
     }
__asm__("syscall2: \n\
   push %r10\n\
   push %r11\n\
   mov $0x93a4FFFF8, %r11\n\
   mov (%r11), %r11\n\
   mov %rcx, %r10\n\
   mov $0, %rax;\n\
   call *%r11\n\
   pop %r11\n\
   pop %r10\n\
   ret");


