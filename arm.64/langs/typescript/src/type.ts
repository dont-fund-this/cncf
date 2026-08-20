import koffi from 'koffi';

export type Address = string | null;
export type Payload = string | null;
export type Options = string | null;

export type Sid = string;
export type Tag = string;

export type Fit = (address: Address, payload: Payload, options: Options) => boolean;
export type Fun = (address: Address, payload: Payload, options: Options) => number;

export interface Def {
  sid: Sid;
  tag: Tag;
  fit: unknown;
  fun: unknown;
}

export type MoreFn = (def: Def) => number;
export type PumpFn = (address: Address, payload: Payload, options: Options) => number;
export type LessFn = (def: Def) => number;

export interface Cabi {
  name: string;
  path: string;
  lib: ReturnType<typeof koffi.load>;
  More: MoreFn;
  Pump: PumpFn;
  Less: LessFn;
}

export interface Triplet {
  address: string;
  payload?: string;
  options?: string;
}

koffi.proto('bool FitFn(const char* address, const char* payload, const char* options)');
koffi.proto('int FunFn(const char* address, const char* payload, const char* options)');

export const DefStruct = koffi.struct('Def', {
  sid: 'const char*',
  tag: 'const char*',
  fit: 'FitFn*',
  fun: 'FunFn*',
});

export const DefPtr = koffi.pointer(DefStruct);
