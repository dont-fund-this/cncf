import { Cabi } from './type.js';
import { find } from './find.js';
import { bind } from './bind.js';

export function boot(targetDir?: string): Cabi[] {
  const engines: Cabi[] = [];

  const envLib = process.env.PAT_LIB;
  if (envLib) {
    const bound = bind(envLib);
    if (bound) {
      engines.push(bound);
      return engines;
    }
  }

  const files = find(targetDir);
  for (const file of files) {
    const bound = bind(file);
    if (bound) {
      engines.push(bound);
    }
  }

  return engines;
}
