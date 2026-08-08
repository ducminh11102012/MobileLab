# MobileLab Android — Linux ARM64

The Linux/ARM64 execution side of MobileLab, built around Android device orchestration, ARM-native workloads, resource-aware scheduling, and optional ARM virtualization.

This is a **Linux application/framework for Android testing**, not an Android APK. Its UI follows the same product language as the existing MobileLab macOS dashboard while replacing Apple-specific runtime assumptions with Linux-native services.

## What is implemented

- Native Qt 6 ARM64 desktop application
- Android target discovery and lifecycle abstraction
- Android emulator integration when the host provides the emulator binary
- ARM64-native userspace execution mode for hosts without nested virtualization
- QEMU AArch64 VM abstraction
- Automatic KVM detection with QEMU TCG fallback
- Resource-aware job scheduler with weighted execution costs
- Local REST API on `127.0.0.1:4000`
- Device, matrix, runtime, scheduler and console views
- Runtime capability probing (`arm64`, KVM, QEMU, Android emulator)
- Device start/stop/restart and shell surfaces
- Backend status reporting suitable for CI and future MCP integration

## Execution model

```text
                    MobileLab Android
                           │
                ┌──────────┴──────────┐
                │   Orchestration API │
                └──────────┬──────────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
          Scheduler    Android       ARM VM
                       Runtime       Backend
              │            │            │
              ▼            ▼            ▼
        Resource Pool   Emulator    QEMU AArch64
              │            │        ┌────┴────┐
              └────────────┴───────►│ KVM / TCG│
                                   └──────────┘
```

The runtime deliberately probes the host before selecting an execution mode. A machine without `/dev/kvm` is not treated as if it has hardware acceleration: virtualization-dependent workloads can fall back to QEMU userspace execution or remain unavailable.

### Important ARM64 hosting constraint

Some hosting environments do not expose nested virtualization. In that situation, Android KVM acceleration cannot be assumed. MobileLab therefore separates **ARM-native execution** from **virtualization** and makes the capability visible to the scheduler and UI.

The framework does not bundle proprietary Android system images or Apple firmware. Android images are expected to be supplied by the operator under their applicable licenses.

## REST API

The local backend currently exposes:

```text
GET  /status
GET  /devices
POST /runs
POST /devices/:id/start
POST /devices/:id/stop
```

Example:

```bash
curl http://127.0.0.1:4000/status
curl http://127.0.0.1:4000/devices
curl -X POST http://127.0.0.1:4000/runs
```

## Build

Requires Qt 6 Widgets + Network and CMake 3.20+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/mobilelab-android
```

For an ARM64 Linux host, build natively or cross-compile with a standard AArch64 C++ toolchain.

## Design relationship to the macOS application

The macOS application remains the primary iOS-facing product. Its SwiftUI dashboard already provides the device grid, test runs, logs, timeline and inspector concepts used by MobileLab. The Linux application keeps those product-level abstractions while implementing the runtime layer with portable Linux components.

This allows the Android/ARM work to be developed and tested independently without pretending that iOS/macOS components can execute on Linux.
