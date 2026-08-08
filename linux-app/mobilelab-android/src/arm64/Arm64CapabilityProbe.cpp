#include "Arm64CapabilityProbe.h"
#include <QFile>
#include <QSysInfo>
#include <QDir>

QJsonObject Arm64CapabilityProbe::probe() const{
    const auto arch=QSysInfo::currentCpuArchitecture();
    const bool kvm=QFile::exists("/dev/kvm");
    const bool binder=QFile::exists("/dev/binder")||QFile::exists("/dev/binderfs");
    const bool memfd=QFile::exists("/proc/sys/vm/mmap_min_addr");
    return {{"architecture",arch},{"aarch64",arch=="arm64"||arch=="aarch64"},{"kvm",kvm},{"binder",binder},{"linux",true},{"container_backend_viable",true},{"memory_probe",memfd}};
}
