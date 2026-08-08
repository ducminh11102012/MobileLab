# Waydroid integration

MobileLab integrates with Waydroid as an Android-container runtime on Linux. This directory contains MobileLab-owned adapters, integration metadata, patches, and compatibility documentation.

The upstream Waydroid project remains the source of the Android container runtime. MobileLab does not claim upstream ownership.

## Integration boundary

- Container lifecycle is exposed through `WaydroidBackend`.
- Host capability detection is handled by the ARM64 runtime layer.
- ADB, package discovery, artifacts, logs, and matrix scheduling remain MobileLab services.
- ARM64-specific behavior is isolated behind capability and profile interfaces.

See `UPSTREAM.md` for provenance and licensing notes.
