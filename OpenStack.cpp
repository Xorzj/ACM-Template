#define OPENSTACK
\ #ifdef OPENSTACK int size = 128 << 20;  // 64MB
char *p = (char *)malloc(size) + size;
#if (defined _WIN64) or (defined __unix)
__asm__("movq %0, %%rsp\n" ::"r"(p));
#else
__asm__("movl %0, %%esp\n" ::"r"(p));
#endif
#endif
\ #ifdef OPENSTACK exit(0);
#else
return 0;
#endif