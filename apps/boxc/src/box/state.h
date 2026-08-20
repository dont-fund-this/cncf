#pragma once

#include "../type.hpp"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace box {

struct Machine {
    void* vm          = nullptr;
    void* console_dev = nullptr;
    void* cs          = nullptr;
    void* drive       = nullptr;
    void* image       = nullptr;
};

struct Shim {
    void*         ptr  = nullptr;
    void*         pr   = nullptr;
    int           fd   = -1;
    std::uint64_t size = 0;
};

struct Doorbell {
    void*                      ptr = nullptr;
    void*                      pr  = nullptr;
    std::vector<std::uint32_t> signals;
};

struct Fb {
    void*         ptr  = nullptr;
    void*         pr   = nullptr;
    std::uint64_t size = 0;
    int           w    = 0;
    int           h    = 0;
};

struct Channel {
    std::string output;
    std::string input;
    std::mutex  io;
};

struct State {
    Machine           machine;
    Shim              shim;
    Doorbell          doorbell;
    Fb                fb;
    Channel           chan;
    std::thread       worker;
    std::atomic<bool> running{false};
};

State& state();

inline constexpr std::uint64_t SHIM_ADDR = 0x100000000ULL;
inline constexpr std::uint64_t DOORBELL_ADDR = 0x200000000ULL;
inline constexpr std::uint64_t FB_ADDR = 0x300000000ULL;

bool boot(const std::string& bios, const std::string& kernel,
          const std::string& initrd, const std::string& drive,
          const std::string& image, const std::string& shim,
          const std::string& fs, const std::string& fb,
          const std::string& cmdline, std::uint64_t ram);
void run();
void pump_input();
void teardown();
void resize(int width, int height);
void on_write(void* opaque, const std::uint8_t* buf, int len);
int  on_read(void* opaque, std::uint8_t* buf, int len);
unsigned char* read_file(const std::string& path, long* out_len);
bool setup_shim(const std::string& path, void* vm);
bool setup_fb(int w, int h, void* vm);
bool setup_doorbell(void* vm);
void poll_doorbell();
void teardown_shim();
std::string shim_suffix(const std::string& shim);

}

extern const Def BoxStart;
extern const Def BoxStop;
extern const Def BoxSend;
extern const Def BoxPoll;
