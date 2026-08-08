#include "WaydroidCommand.h"
namespace mobilelab::waydroid {
std::string WaydroidCommand::build(const std::vector<std::string>& args) {
    std::string out="waydroid";
    for(const auto& a:args) out += " '"+a+"'";
    return out;
}
}
