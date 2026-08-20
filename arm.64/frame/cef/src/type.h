#pragma once

#include <string>
#include <vector>

using Address = const char*;
using Payload = const char*;
using Options = const char*;

using FitFn = bool (*)(Address, Payload, Options);
using FunFn = int (*)(Address, Payload, Options);

struct Def {
    Address sid;
    Address tag;
    FitFn   fit;
    FunFn   fun;
};

using MoreFn = int (*)(Def);
using PumpFn = int (*)(Address, Payload, Options);
using LessFn = int (*)(Def);

struct Cabi {
    std::string name;
    std::string path;
    void*       lib;
    MoreFn      More;
    PumpFn      Pump;
    LessFn      Less;
};

struct Triplet {
    std::string address;
    std::string payload;
    std::string options;
};
