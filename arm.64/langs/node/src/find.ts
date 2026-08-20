import { readdirSync, statSync, existsSync } from 'node:fs';
import { resolve, join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';

const __dirname = dirname(fileURLToPath(import.meta.url));

export function find(targetDir?: string): string[] {
  let dir = targetDir;
  if (!dir) {
    if (process.env.DIST_DIR) {
      dir = process.env.DIST_DIR;
    } else {
      const candidates = [
        resolve(__dirname, '../../../dist'),
        resolve(__dirname, '../../dist'),
        resolve(process.cwd(), 'dist'),
        'dist',
      ];
      dir = candidates.find(c => existsSync(c)) || 'dist';
    }
  }

  if (!existsSync(dir)) return [];

  try {
    return readdirSync(dir)
      .map(name => join(dir!, name))
      .filter(p => statSync(p).isFile() && !p.endsWith('.DS_Store'));
  } catch {
    return [];
  }
}
