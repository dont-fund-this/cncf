module main

pub type Address = &u8
pub type Payload = &u8
pub type Options = &u8
pub type Sid = &u8
pub type Tag = &u8

pub type FitFn = fn (Address, Payload, Options) bool
pub type FunFn = fn (Address, Payload, Options) int

pub struct Def {
pub:
	sid &u8 = unsafe { 0 }
	tag &u8 = unsafe { 0 }
	fit ?FitFn
	fun ?FunFn
}

pub type PumpFn = fn (Address, Payload, Options) int

pub struct Cabi {
pub:
	name string
	path string
	pump ?PumpFn
}

pub struct Triplet {
pub:
	address string
	payload string
	options string
}
