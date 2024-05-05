#include <iostream>
#include <ctime>
#include <dlfcn.h>
#include <sys/time.h>

extern "C" {
    void __cyg_profile_func_enter(void *this_func, void *call_site) __attribute__((no_instrument_function));
    void __cyg_profile_func_exit(void *this_func, void *call_site) __attribute__((no_instrument_function));
}

void __attribute__((no_instrument_function)) log_function_call(const char *action, void *func, void *caller) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    Dl_info func_info, caller_info;
    const char *func_symname = "<unknown>";
    const char *caller_symname = "<unknown>";

    // Resolve function name from the function address
    if (dladdr(func, &func_info) && func_info.dli_sname) {
        func_symname = func_info.dli_sname;
    }

    // Resolve caller function name from the caller address
    if (dladdr(caller, &caller_info) && caller_info.dli_sname) {
        caller_symname = caller_info.dli_sname;
    }

    std::cout << tv.tv_sec << "." << tv.tv_usec << " - " << action 
              << " function: " << func_symname << " (" << func << ")"
              << " called by: " << caller_symname << " (" << caller << ")" << std::endl;
}

void __cyg_profile_func_enter(void *this_func, void *call_site) {
    log_function_call("Enter", this_func, call_site);
}

void __cyg_profile_func_exit(void *this_func, void *call_site) {
    log_function_call("Exit", this_func, call_site);
}

