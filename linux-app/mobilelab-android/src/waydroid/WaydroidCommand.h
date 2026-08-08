#pragma once
#include <string>
#include <vector>
namespace mobilelab::waydroid {
class WaydroidCommand {
public:
    static std::string build(const std::vector<std::string>& args);
};
}
