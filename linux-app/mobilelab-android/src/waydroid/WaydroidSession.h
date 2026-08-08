#pragma once
#include <string>

namespace mobilelab::waydroid {
struct WaydroidSession {
    std::string name;
    std::string container;
    std::string state;
    int adbPort{5555};
};
}
