#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <dlfcn.h>

typedef int (*PumpFn)(const char*, const char*, const char*);

CAMLprim value caml_dlopen(value v_path) {
    CAMLparam1(v_path);
    void* handle = dlopen(String_val(v_path), RTLD_LAZY | RTLD_LOCAL);
    if (!handle) CAMLreturn(Val_int(0));
    CAMLreturn(caml_alloc_some((value)handle));
}

CAMLprim value caml_dlsym_pump(value v_handle) {
    CAMLparam1(v_handle);
    void* handle = (void*)Some_val(v_handle);
    void* sym = dlsym(handle, "Pump");
    if (!sym) CAMLreturn(Val_int(0));
    CAMLreturn(caml_alloc_some((value)sym));
}

CAMLprim value caml_call_pump(value v_fn, value v_addr, value v_pay, value v_opt) {
    CAMLparam4(v_fn, v_addr, v_pay, v_opt);
    PumpFn fn = (PumpFn)Some_val(v_fn);
    int res = fn(String_val(v_addr), String_val(v_pay), String_val(v_opt));
    CAMLreturn(Val_int(res));
}
