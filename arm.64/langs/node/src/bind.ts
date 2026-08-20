import path from 'node:path';
import koffi from 'koffi';
import { Cabi, DefStruct, MoreFn, PumpFn, LessFn } from './type.js';

export function bind(binaryPath: string): Cabi | null {
  const filename = path.basename(binaryPath);
  if (['c', 'cpp', 'rust', 'go', 'swift', 'haskell', 'zig', 'v', 'slint_sample'].includes(filename)) {
    return null;
  }

  try {
    const lib = koffi.load(binaryPath);

    const More = lib.func('More', 'int', [koffi.pointer(DefStruct)]) as MoreFn;
    const Pump = lib.func('Pump', 'int', ['str', 'str', 'str']) as PumpFn;
    const Less = lib.func('Less', 'int', [koffi.pointer(DefStruct)]) as LessFn;

    return {
      name: filename,
      path: binaryPath,
      lib,
      More,
      Pump,
      Less,
    };
  } catch {
    return null;
  }
}
