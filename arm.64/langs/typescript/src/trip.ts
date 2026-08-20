import { Triplet } from './type.js';

export function trip(): Triplet[] {
  return [
    { address: '/version', payload: '{}', options: '{"once":true}' },
    { address: '/storage', payload: '{}', options: '{"once":true}' },
    { address: 'sql.help', payload: '{}', options: '{"once":true}' },
  ];
}
