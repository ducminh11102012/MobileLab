# ARM64 Android container backend

MobileLab's Linux Android backend can use Waydroid as a container runtime. This is specifically useful for ARM64 Linux hosts where nested virtualization is unavailable.

## Runtime model

```text
MobileLab
   -> AndroidContainerRuntime
      -> WaydroidBackend
         -> Waydroid container
            -> Android system/vendor images
               -> ADB / tests / artifacts
```

The backend does not require KVM for the container path. KVM may still be detected and reported as an optional host capability.

## What runs on the ARM64 host

- MobileLab orchestration and scheduling
- ARM64 capability detection
- Waydroid lifecycle integration
- Android container lifecycle
- ADB-based shell and diagnostics
- test matrix execution
- screenshots, logcat and run artifacts

## Upstream boundary

Waydroid remains an external upstream dependency. MobileLab owns the orchestration, ARM64 capability layer, lifecycle adapter, matrix execution and artifact pipeline.

## OSL / Ampere target

The intended target is an ARM64 Linux VM. No Apple firmware, iOS images, Apple virtualization stack, or device-restriction circumvention is part of this backend.
