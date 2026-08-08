#pragma once
#include "Arm64RuntimeProfile.h"
namespace mobilelab::arm64 {
class Arm64ContainerPolicy {
public:
    bool supportsUserspaceAndroid(const Arm64RuntimeProfile& p) const;
    bool supportsNestedKvm(const Arm64RuntimeProfile& p) const;
};
}
