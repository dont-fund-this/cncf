import { boot } from './boot.js';
import { trip } from './trip.js';

const dist = boot(process.argv[2]);

if (dist.length > 0) {
  const trips = trip();
  for (const d of dist) {
    for (const t of trips) {
      d.Pump(t.address, t.payload ?? '{}', t.options ?? '{"once":true}');
    }
  }
}

const out = {
  lang: 'typescript',
  status: 'ready',
  engines: dist.length,
};

console.log(JSON.stringify(out, null, 2));
