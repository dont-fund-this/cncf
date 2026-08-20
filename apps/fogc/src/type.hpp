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
    Sid sid;
    Tag tag;
    Fit fit;
    Fun fun;
};

typedef const Def* Defs;

struct Trip {
    Address address;
    Payload payload;
    Options options;
};

constexpr const char* NOT_IMPLEMENTED_JSON = "{\"text\":\"noop\"}";
