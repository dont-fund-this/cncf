import { awaits } from './awaits.js';

export function ondata(address, payload, options) {
  const waiter = awaits.get(address);
  if (!waiter) return 0;
  waiter.packets.push({ address, payload, options });
  return 1;
}
