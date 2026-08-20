#pragma once
#include <string>
#include <vector>

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

typedef int (*MoreFn)(Def def);
typedef int (*PumpFn)(Address address, Payload payload, Options options);
typedef int (*LessFn)(Def def);

struct Cabi {
    std::string name;
    std::string path;
    void* handle = nullptr;
    MoreFn More = nullptr;
    PumpFn Pump = nullptr;
    LessFn Less = nullptr;
};

struct Triplet {
    std::string address;
    std::string payload;
    std::string options;
};
