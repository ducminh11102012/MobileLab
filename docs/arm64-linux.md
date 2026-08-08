# ARM64 Linux development

MobileLab keeps platform-specific execution backends separate from the orchestration layer. The ARM64 Linux path is intended for native Linux/ARM64 development, Android-related tooling that does not require nested virtualization, and architecture-specific build/test validation.

The backend exposes `GET /capabilities` to report the host architecture, Linux ARM64 status, kernel/runtime information, and whether `/dev/kvm` is available. This lets the scheduler and future Android backend distinguish native ARM64 execution from virtualization-dependent workloads instead of assuming that every Linux ARM64 host provides KVM.

The current implementation deliberately does **not** attempt to start a VM or Android emulator. In particular, the absence of `/dev/kvm` is treated as a normal capability constraint rather than something to work around.

This is an initial ARM64-native foundation and is expected to grow as Android build/test workflows are implemented.
