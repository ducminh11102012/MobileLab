#pragma once
#include <string>
namespace mobilelab::arm64 {
struct Arm64RuntimeProfile {
    std::string cpu;
    bool kvm{false};
    bool binder{false};
    bool ashmem{false};
    bool containerReady{false};
};
}
