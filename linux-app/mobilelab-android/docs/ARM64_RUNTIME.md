# ARM64 Runtime Architecture

MobileLab's Android backend treats ARM64 as a first-class execution target rather than as an x86 environment with an Android layer on top.

## Why ARM64 is useful

For ARM-native Android workloads, the host and guest can share the same ISA. This makes it possible to validate ARM64 native libraries, ABI selection, CPU feature detection, JNI behavior, and architecture-specific performance without making x86 translation the default.

## Capability tiers

- `native-arm64`: ARM64 host + ARM64 Android image.
- `userspace-arm64`: ARM64 Android execution without nested KVM acceleration.
- `accelerated-arm64`: ARM64 guest with host KVM when the provider exposes it.
- `translated-x86`: explicitly marked as a compatibility path, never silently selected for ARM validation.

The runtime reports the selected tier to the UI, scheduler, REST API, and test artifacts.

## OSL / Ampere compatibility

The design does not require nested virtualization. If `/dev/kvm` is unavailable, MobileLab keeps Android workloads in a userspace/native-compatible path and reports the limitation instead of claiming KVM acceleration.

System images and Google Android Emulator binaries are provisioned locally from the official Android SDK distribution. They are not vendored into this repository.
