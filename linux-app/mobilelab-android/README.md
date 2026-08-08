# MobileLab Android — Linux ARM64 UI

MobileLab Android is the Linux/ARM64 desktop control surface for MobileLab's Android execution work.

It is intentionally a Linux application, not an Android APK. The UI is designed around Android device orchestration, Android test matrices, native ARM64 workloads, resource scheduling, and backend observability.

The layout follows the same product language as the existing MobileLab macOS dashboard: navigator sidebar, workspace toolbar, device pool, matrix workspace, live console, and right-side inspector. The implementation uses Qt 6 so it can run natively on ARM64 Linux without depending on macOS frameworks or Apple hardware.

## Build

Requires Qt 6 Widgets and Network plus CMake 3.20+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/mobilelab-android
```

## Current scope

- ARM64 Linux desktop UI
- Android-oriented device pool
- Test matrix view
- Native ARM64 execution status
- Host resource view
- Live scheduler/backend console
- Device inspector and actions surface
- Dark desktop UI matching the MobileLab product language

The UI is currently a front-end foundation. Device actions are intentionally represented as application surfaces until they are connected to the MobileLab orchestration API.
