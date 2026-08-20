import { boot } from './boot.js';

export function bind(libDir) {
  const lib = boot(libDir);
  if (!lib) return null;
  return {
    lib,
    Attach: lib.func('bool Attach(void*)'),
    Detach: lib.func('bool Detach()'),
    Invoke: lib.func('int Invoke(const char*, const char*, const char*)'),
    Report: lib.func('bool Report(_Out_ Info* out)'),
  };
}
