typedef void (*init_func)(void);
//linux
void run_hooks();
extern "C" void do_global_ctors()
{
    run_hooks();
}


