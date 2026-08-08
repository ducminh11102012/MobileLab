#include "WaydroidImageManager.h"
namespace mobilelab::waydroid {
std::vector<std::filesystem::path> WaydroidImageManager::discover(const std::filesystem::path& root) const {
    std::vector<std::filesystem::path> result;
    if(!std::filesystem::exists(root)) return result;
    for(const auto& e:std::filesystem::directory_iterator(root))
        if(e.is_regular_file()) result.push_back(e.path());
    return result;
}
}
