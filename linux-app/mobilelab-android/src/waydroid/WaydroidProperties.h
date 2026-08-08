#pragma once
#include <map>
#include <string>
namespace mobilelab::waydroid {
class WaydroidProperties {
public:
    static std::map<std::string,std::string> parse(const std::string& text);
};
}
