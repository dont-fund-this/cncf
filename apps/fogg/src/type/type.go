package types

import "unsafe"

type Address = unsafe.Pointer
type Payload = unsafe.Pointer
type Options = unsafe.Pointer

type Sid = unsafe.Pointer
type Tag = unsafe.Pointer
type Fit = func(address Address, payload Payload, options Options) bool
type Fun = func(address Address, payload Payload, options Options) int

type Def struct {
	Sid Sid
	Tag Tag
	Fit Fit
	Fun Fun
}

type Defs = []Def

type Trip struct {
	Address Address
	Payload Payload
	Options Options
}

const NotImplementedJSON = "{\"text\":\"noop\"}"
