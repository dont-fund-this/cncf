import koffi from 'koffi';
import { find } from './find.js';
import './type.js';

export function boot(libDir) {
  for (const file of find(libDir)) {
    let lib;
    try { lib = koffi.load(file); } catch { continue; }

    let report;
    try { report = lib.func('bool Report(_Out_ Info* out)'); } catch { continue; }

    const info = {};
    try { if (!report(info)) continue; } catch { continue; }

    if (info.plugin_type === 'jam') return lib;
  }
  return null;
}
