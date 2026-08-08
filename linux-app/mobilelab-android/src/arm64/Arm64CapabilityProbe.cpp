#include "Arm64CapabilityProbe.h"
#include <QFile>
#include <QSysInfo>
#include <QDir>

QJsonObject Arm64CapabilityProbe::probe() const{
    const auto arch=QSysInfo::currentCpuArchitecture();
    const bool aarch64=arch=="arm64"||arch=="aarch64";
    const bool kvm=QFile::exists("/dev/kvm");
    const bool binder=QFile::exists("/dev/binder")||QFile::exists("/dev/binderfs");
    const bool cgroup=QFile::exists("/sys/fs/cgroup");
    const bool lxc=QFile::exists("/usr/bin/lxc-start")||QFile::exists("/usr/sbin/lxc-start");
    const bool proc=QFile::exists("/proc/self/status");
    return {
        {"architecture",arch},
        {"aarch64",aarch64},
        {"kvm",kvm},
        {"binder",binder},
        {"cgroup_v2_or_mount",cgroup},
        {"lxc_tools",lxc},
        {"procfs",proc},
        {"linux",true},
        {"nested_virtualization_required",false},
        {"container_backend_viable",aarch64&&binder&&cgroup&&proc}
    };
}
