# MobileLab Android ARM64 — End-to-End Demo

This path is designed for ARM64 Linux hosts such as Ampere systems. It uses the Android SDK command-line tools and Google's Android Emulator where available; MobileLab does not redistribute Android system images.

## 1. Provision the SDK

Install the Android command-line tools, platform-tools, emulator, and an ARM64-v8a system image using the normal Android SDK package flow.

## 2. Verify the host

```bash
ctest --test-dir build -R mobilelab-arm64-smoke --output-on-failure
```

The smoke test reports CPU architecture, KVM availability, ADB/emulator availability, and the installed system-image tree. KVM is optional for the userspace ARM64 execution path.

## 3. Create an AVD

From the MobileLab UI choose **New AVD**, select an installed ARM64 system image, and create the device profile through `avdmanager`.

The same operation is available programmatically through `AndroidEmulator::createAvd()`.

## 4. Run a matrix

Choose **Run Matrix**. MobileLab will:

1. discover AVDs from the Android SDK;
2. submit each target to the resource scheduler;
3. boot the ARM64 AVD;
4. wait for the Android device through ADB;
5. collect logcat and a PNG screenshot;
6. stop the emulator;
7. write a machine-readable `run.json` report.

Artifacts are written below the configured `artifacts/` directory.

## ARM64-specific behavior

MobileLab records host architecture and KVM availability and avoids treating nested virtualization as a hard prerequisite. This makes the execution model suitable for ARM64 Linux environments where KVM cannot be exposed to the guest.

## Scope

The implementation is intentionally focused on Android/ARM64 Linux. It does not execute Apple firmware, iOS images, or Apple-specific virtualization workflows on the ARM64 host.
