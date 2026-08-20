import { awaits, abi } from './awaits.js';

export function stop() {
  if (abi.value) abi.value.Detach();
  awaits.clear();
  abi.value = null;
}
