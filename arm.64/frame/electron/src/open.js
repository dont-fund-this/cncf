import koffi from 'koffi';
import { bind } from './bind.js';
import { ondata } from './ondata.js';
import { abi } from './awaits.js';

let registered = null;

export function open(libDir) {
  const bound = bind(libDir);
  if (!bound) return false;
  registered ??= koffi.register(ondata, 'CallbackFn*');
  if (!bound.Attach(registered)) return false;
  abi.value = bound;
  return true;
}
