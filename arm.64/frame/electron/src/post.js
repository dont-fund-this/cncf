import { awaits, abi } from './awaits.js';

const PREFIX = 'cabi.reply.';
const TIMEOUT = 50;
let next = 1;

export function post(address, payload, options) {
  const into = PREFIX + (next++).toString(36);
  const opts =
    typeof options === 'string' ? (options ? JSON.parse(options) : {})
    : (options && typeof options === 'object') ? { ...options }
    : {};
  const ms = typeof opts.timeout === 'number' && opts.timeout > 0 ? opts.timeout : TIMEOUT;
  delete opts.timeout;
  opts.into = into;
  const pay = typeof payload === 'string' ? payload : JSON.stringify(payload ?? {});
  const opt = JSON.stringify(opts);

  const none = opts.none === true || !(opts.once === true || opts.many === true);

  if (none) {
    const count = abi.value.Invoke(address, pay, opt);
    if (count < 0) return Promise.reject(new Error(`post(${address}) error ${count}`));
    return Promise.resolve([{ address: into, payload: JSON.stringify({ ok: true, fit: count }), options: opt }]);
  }

  return new Promise((resolve, reject) => {
    const waiter = { packets: [] };
    awaits.set(into, waiter);
    const timer = setTimeout(() => {
      awaits.delete(into);
      resolve(waiter.packets);
    }, ms);
    let rc;
    try {
      rc = abi.value.Invoke(address, pay, opt);
    } catch (error) {
      awaits.delete(into);
      clearTimeout(timer);
      reject(error);
      return;
    }
    awaits.delete(into);
    clearTimeout(timer);
    if (rc < 0) { reject(new Error(`post(${address}) error ${rc}`)); return; }
    resolve(waiter.packets);
  });
}
