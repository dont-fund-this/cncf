package main

/*
typedef const char* Address;
typedef const char* Payload;
typedef const char* Options;
typedef const char* Sid;
typedef const char* Tag;

typedef int (*FitFn)(Address, Payload, Options);
typedef int (*FunFn)(Address, Payload, Options);

typedef struct {
    Sid sid;
    Tag tag;
    FitFn fit;
    FunFn fun;
} Def;
*/
import "C"
import "unsafe"

type Def struct {
	Sid string
	Tag string
}

type Cabi struct {
	Name   string
	Path   string
	Handle unsafe.Pointer
	PumpFn unsafe.Pointer
}

type Triplet struct {
	Address string
	Payload string
	Options string
}
