#pragma once
#include <filesystem>
#include <vector>
namespace mobilelab::waydroid {
class WaydroidImageManager {
public:
    std::vector<std::filesystem::path> discover(const std::filesystem::path& root) const;
};
}
