#include "Arm64ContainerPolicy.h"
namespace mobilelab::arm64 {
bool Arm64ContainerPolicy::supportsUserspaceAndroid(const Arm64RuntimeProfile& p) const { return p.binder && p.containerReady; }
bool Arm64ContainerPolicy::supportsNestedKvm(const Arm64RuntimeProfile& p) const { return p.kvm; }
}
