#pragma once
#include <cstddef>
#include <string_view>

typedef const char* Address;
typedef const char* Payload;
typedef const char* Options;

typedef const char* Sid;
typedef const char* Tag;
typedef bool (*Fit)(Address address, Payload payload, Options options);
typedef int (*Fun)(Address address, Payload payload, Options options);

struct Def {
    Sid sid = nullptr;
    Tag tag = nullptr;
    Fit fit = nullptr;
    Fun fun = nullptr;
};

typedef Def def;
typedef const Def* Defs;

Defs with();
size_t with_count();

struct Trip {
    Address address;
    Payload payload;
    Options options;
};

constexpr const char* NOT_IMPLEMENTED_JSON = "{\"text\":\"noop\"}";
