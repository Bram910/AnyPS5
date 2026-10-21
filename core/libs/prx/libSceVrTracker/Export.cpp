#include <cstdint>
#include <cstddef>
#include "SceTypes.hpp"
#include "prx/libc/include/General.hpp"

struct VrTrackerMemoryBlock {
    void* address;
    std::uint32_t size;
    std::uint32_t alignment;
};

struct VrTrackerInitConfig {
    std::uint32_t type;
    std::uint32_t option;
    std::uint32_t mode;
    std::uint32_t parameter;
    std::uint32_t reserved[4];
};

struct VrTrackerInitParam {
    std::uint32_t size;
    std::int32_t mode;
    std::uint32_t reserved[6];
    VrTrackerInitConfig config;
    VrTrackerMemoryBlock memory[3];
    std::uint32_t option112;
    std::uint32_t option116;
    std::uint32_t reservedTail[6];
};

static_assert(sizeof(VrTrackerMemoryBlock) == 16);
static_assert(sizeof(VrTrackerInitConfig) == 32);
static_assert(sizeof(VrTrackerInitParam) == 144);
static_assert(offsetof(VrTrackerInitParam, config) == 32);
static_assert(offsetof(VrTrackerInitParam, memory) == 64);
static_assert(offsetof(VrTrackerInitParam, option112) == 112);
struct VrTrackerGpuSubmitParam;
struct VrTrackerResult;

struct VrTrackerCpuProcessParam {
    std::uint32_t size;
    std::uint32_t mode;
    std::uint32_t argument;
    std::uint32_t reserved[5];
};

struct VrTrackerGpuWaitParam {
    std::uint32_t size;
    std::byte payload[28];
};

struct VrTrackerGetResultParam {
    std::uint32_t size;
    std::int32_t handle;
    std::uint32_t mode;
    std::uint32_t extendedResult;
    std::uint64_t time;
    std::uint32_t statusMode;
    std::uint32_t reserved0;
    std::uint32_t option32;
    std::byte outputValue[4];
    std::uint32_t option40;
    std::uint32_t reserved1[2];
    std::byte unused[4];
};

static_assert(sizeof(VrTrackerCpuProcessParam) == 32);
static_assert(offsetof(VrTrackerCpuProcessParam, mode) == 4);
static_assert(offsetof(VrTrackerCpuProcessParam, argument) == 8);
static_assert(sizeof(VrTrackerGpuWaitParam) == 32);
static_assert(sizeof(VrTrackerGetResultParam) == 56);
static_assert(offsetof(VrTrackerGetResultParam, time) == 16);
static_assert(offsetof(VrTrackerGetResultParam, statusMode) == 24);
static_assert(offsetof(VrTrackerGetResultParam, outputValue) == 36);
static_assert(offsetof(VrTrackerGetResultParam, reserved1) == 44);

extern "C" {

int APS5_VABI sceVrTrackerCpuProcess(const VrTrackerCpuProcessParam* param) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}
int APS5_VABI sceVrTrackerGetResult(const VrTrackerGetResultParam* param, VrTrackerResult* result) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}
int APS5_VABI sceVrTrackerGetTime(std::uint64_t* time) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}
int APS5_VABI sceVrTrackerGpuSubmit(const VrTrackerGpuSubmitParam* param) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}
int APS5_VABI sceVrTrackerGpuWait(const VrTrackerGpuWaitParam* param) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}
int APS5_VABI sceVrTrackerInit(const VrTrackerInitParam* param) {
    NotImplemented_nid_no_patch(__func__);
    return 0;
}

}
