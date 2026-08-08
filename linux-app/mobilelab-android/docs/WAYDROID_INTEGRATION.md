# Waydroid integration

MobileLab's Linux Android backend uses [Waydroid](https://github.com/waydroid/waydroid) as its Android-container execution layer.

The upstream source is pinned as a Git submodule under `third_party/waydroid`. MobileLab does not vendor a renamed copy of the Waydroid code or pretend that its implementation is part of the MobileLab project.

## Runtime architecture

```text
MobileLab UI / API
        |
        v
AndroidContainerRuntime
        |
        +-- Arm64CapabilityProbe
        +-- WaydroidBackend
        +-- image/catalog services
        +-- scheduler + matrix executor
        +-- logs / artifacts
        |
        v
Waydroid CLI + services
        |
        v
LXC / Android userspace
```

`WaydroidBackend` is intentionally a process/service adapter. It discovers the installed `waydroid` executable and controls initialization, container lifecycle, session lifecycle, and shell access through the upstream interface.

## ARM64-specific design

The target OSL environment is ARM64 Linux and does not provide nested virtualization. The MobileLab backend therefore treats container execution as the primary Android path and does not require `/dev/kvm`.

Before starting a run, `Arm64CapabilityProbe` checks the host architecture and basic container prerequisites such as binder availability, cgroup support, and procfs. The scheduler can use this diagnostic information to reject an incompatible node before allocating a test workload.

This is deliberately different from an x86 Android VM design: the Android userspace is intended to execute natively as ARM64 code inside a Linux container rather than relying on a nested hypervisor.

## Upstream revision

The submodule is pinned to:

`e7d73e7ff9d23003356d716e5a40fa5ca7ad17e0`

This makes the integration reproducible while still tracking the upstream project as a separate component.

## License boundary

Waydroid is distributed under the GNU GPLv3. Its source, license, copyright notices, and upstream history remain separate from MobileLab's MIT-licensed code. The MobileLab repository does not relicense the Waydroid source.

When building or redistributing a combined distribution, follow the license terms of each component and retain the applicable notices.

## Development setup

```bash
git submodule update --init --recursive linux-app/mobilelab-android/third_party/waydroid
```

A host with Waydroid installed can be probed by the MobileLab Android runtime. The application does not download Android system images or proprietary Google packages as part of this integration.
