import { readdirSync, realpathSync } from 'node:fs';
import { dirname, join } from 'node:path';

export function find(libDir) {
  let dir = libDir;
  if (!dir) {
    let p = realpathSync(process.execPath);
    while (!p.endsWith('.app') && p !== dirname(p)) p = dirname(p);
    if (!p.endsWith('.app')) return [];
    dir = dirname(p);
  }
  try { return readdirSync(dir).map(name => join(dir, name)); }
  catch { return []; }
}
