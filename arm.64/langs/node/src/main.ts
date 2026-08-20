import { boot } from './boot.js';
import { trip } from './trip.js';

const targetDir = process.argv[2];
const dist = boot(targetDir);

if (dist.length > 0) {
  const trips = trip();
  for (const d of dist) {
    for (const t of trips) {
      d.Pump(t.address, t.payload ?? '{}', t.options ?? '{"once":true}');
    }
  }
}

const out = {
  lang: 'node',
  status: 'ready',
  engines: dist.length,
};

console.log(JSON.stringify(out, null, 2));
