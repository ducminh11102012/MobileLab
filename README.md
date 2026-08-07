# MobileLab (Formerly IOSLab)

> **Cross-platform mobile testing and device orchestration for modern development workflows.**

**MobileLab** is an open-source platform for orchestrating large-scale mobile application testing across multiple devices, operating-system versions, and execution environments.

The project began as **iOSLab**, a macOS-focused platform designed to run large numbers of iOS Simulator instances in parallel. It is now evolving into a broader cross-platform mobile testing platform, with **Android support currently under development and testing**.

> **Project status:** iOS support is the current primary platform. Android support is under active development and internal testing and is **not yet publicly released**.

---

## Formerly iOSLab

MobileLab was originally created under the name **iOSLab**.

The original project focused on solving a specific problem: running large iOS Simulator test matrices concurrently on a single Mac instead of executing each device/OS configuration sequentially.

As the architecture evolved, the project expanded beyond iOS-specific orchestration toward a more general mobile testing platform capable of supporting multiple mobile operating systems and execution backends.

The repository, existing source code, historical commits, release artifacts, and some internal interfaces may therefore still contain references to:

* `iOSLab`
* `ioslab`
* iOS-specific module names
* legacy CLI commands
* earlier architecture terminology

These references are retained during the transition and do **not** represent the long-term project naming.

**MobileLab is the current project name.**

---

## Why MobileLab?

Mobile applications increasingly need to be validated across a large matrix of:

* device models
* operating-system versions
* screen sizes
* architectures
* runtime configurations
* hardware capabilities
* application states

Running every configuration sequentially quickly becomes impractical.

MobileLab is designed around **parallel execution and centralized orchestration**, allowing multiple mobile environments to be created, scheduled, tested, observed, and managed as a unified device pool.

Instead of thinking about testing as:

```text
Build
  ↓
Device A
  ↓
Device B
  ↓
Device C
  ↓
Device D
```

MobileLab is designed around:

```text
                    ┌── Device A
                    ├── Device B
Test Matrix ────────┼── Device C
                    ├── Device D
                    ├── Device E
                    └── ...
```

This makes large device/OS matrices significantly easier to automate and integrate into local development and CI workflows.

---

# Current Platform Status

| Platform                         | Status                         | Execution                                             |
| -------------------------------- | ------------------------------ | ----------------------------------------------------- |
| **iOS**                          | 🟢 Active                      | iOS Simulator / experimental virtualized environments |
| **Android**                      | 🟡 Under Development & Testing | Android emulator / ARM-focused development            |
| **Cross-platform orchestration** | 🟡 In Development              | Unified device and job model                          |
| **Distributed execution**        | 🔵 Planned                     | Multi-machine / multi-node execution                  |

### Android support

Android support is currently being developed as part of the project's expansion beyond its original iOS-only architecture.

The Android implementation is currently being used for **development, experimentation, and internal testing**.

It is **not yet considered a public stable feature** and should not be treated as production-ready.

The long-term goal is to provide Android environments through the same orchestration concepts used by the iOS side of MobileLab, rather than maintaining two completely separate testing systems.

---

# Features

## Parallel execution

Run large numbers of simulator or emulator instances concurrently on a single host.

```bash
ioslab test --devices=32
```

The legacy `ioslab` command remains available while the project transitions to the MobileLab naming.

---

## Device and OS matrix testing

Define a testing matrix across multiple device models and operating-system versions.

```yaml
test:
  devices: 24
  ios_versions: [16, 17, 18]
```

MobileLab handles scheduling, retries, and resource-aware distribution across the available environments.

---

## Hybrid device platform

The current architecture supports multiple types of mobile execution environments.

For iOS, this includes:

* iOS Simulator instances
* experimental virtualized iOS environments
* unified device management
* VM-aware resource scheduling

Android support is being developed around the same broader device-orchestration architecture.

---

## Resource-aware scheduling

Different execution environments have different resource requirements.

MobileLab therefore assigns resource weights to execution targets and uses host CPU and memory availability when determining concurrency.

For example:

```text
Simulator    → cost weight 1
VM           → cost weight 4
```

This prevents the orchestrator from blindly starting more environments than the host can reasonably handle.

---

## Visual regression testing

Capture screenshots from test states and compare them against known baselines.

This allows automated detection of:

* layout changes
* rendering differences
* OS-specific UI changes
* unexpected visual regressions

---

## Cross-version testing

Compare application behavior and visual output across different operating-system versions.

This is particularly useful for identifying regressions that only appear on specific OS releases.

---

## Test orchestration

Describe the desired testing environment declaratively.

MobileLab manages:

* device allocation
* test scheduling
* retries
* load balancing
* resource limits
* result collection

---

## REST API

MobileLab exposes APIs for integrating device orchestration into external tooling and CI/CD systems.

Example endpoints include:

```text
POST /devices/spawn
POST /tests/run
GET  /metrics
```

Virtualized environments additionally expose VM-specific operations where supported:

```text
POST /vms/spawn
GET  /vms
GET  /vms/{id}/screenshot
POST /vms/{id}/input
POST /vms/{id}/backup
POST /vms/{id}/restore
POST /vms/{id}/switch
```

---

## Model Context Protocol

MobileLab includes an MCP interface that allows compatible AI development agents to interact with the device pool.

Supported operations include:

```text
list_devices
spawn_device
run_test
get_screenshot
inject_input
```

This makes it possible for development agents to interact with mobile testing environments programmatically rather than relying exclusively on manual workflows.

---

## AI-assisted exploratory testing

The platform is designed to support AI-assisted exploration of mobile applications using live device state and screenshots.

This area remains experimental and is subject to ongoing development.

---

## Local CI pipeline

Run build, test, validation, and reporting workflows locally without requiring every operation to pass through an external CI queue.

```bash
ioslab run pipeline.yaml
```

---

# iOS Virtualization

One of the project's experimental areas is running genuine iOS environments through virtualization on supported Apple Silicon systems.

The current architecture contains a VM lifecycle pipeline including:

```text
fw_prepare
    ↓
fw_patch
    ↓
restore
    ↓
cfw_install
    ↓
boot
```

Virtualized iOS environments are treated differently from lightweight Simulator instances because of their substantially higher resource requirements.

VM functionality should currently be considered **experimental**.

---

# Android Development

Android support represents the next major stage of MobileLab's development.

The goal is to provide a unified architecture where Android environments can eventually participate in the same concepts as iOS environments:

```text
                    MobileLab
                        │
              ┌─────────┴─────────┐
              │                   │
             iOS               Android
              │                   │
       ┌──────┴──────┐      ┌─────┴─────┐
       │             │      │           │
   Simulator         VM   Emulator    ARM
```

Current Android work includes experimentation with:

* Android emulator orchestration
* ARM-based execution environments
* device lifecycle management
* unified device models
* resource-aware scheduling
* cross-platform testing infrastructure

### Important

Android functionality is currently:

**Under Development & Testing**

It has **not been publicly released as a stable MobileLab feature**.

Interfaces and implementation details may change substantially before public availability.

---

# Requirements

### Current iOS development

* macOS
* Xcode
* Apple Silicon recommended
* 16 GB RAM minimum
* 32 GB+ recommended for high simulator counts

### Android development

Android support is still under development. Requirements may change as the Android execution backend matures.

ARM-based environments are an active area of investigation and development.

---

# Installation

## Prebuilt releases

Precompiled application bundles and CLI tools are available through the project's release system.

During the transition from iOSLab to MobileLab, some existing releases may still use the legacy `ioslab` naming.

---

## Build from source

Clone the repository and build the project:

```bash
git clone <repository-url>
cd <repository-directory>
make setup
```

To build without a connected simulator environment:

```bash
IOSLAB_SIMULATOR_MOCK=true ./scripts/build_release.sh
```

The `IOSLAB_*` environment variables are currently retained for backward compatibility with the original iOSLab implementation.

---

# Usage

## Run parallel iOS Simulator tests

```bash
ioslab test --devices=12
```

Run a larger matrix:

```bash
ioslab test --devices=32
```

The legacy CLI name is intentional during the project transition.

Future releases may introduce the new `mobilelab` command namespace.

---

# Matrix Configuration

A basic iOS matrix can be described as:

```yaml
test:
  devices: 24
  ios_versions: [16, 17, 18]
```

The longer-term cross-platform model is designed around a unified device definition:

```yaml
test:
  devices:
    - type: ios-simulator
      count: 12
      os_versions: [17, 18]

    - type: android-emulator
      count: 12
      os_versions: [14, 15]
```

> Android matrix configuration is currently **design/development work** and is not yet publicly available as a stable feature.

---

# Virtual Machine Lifecycle

For supported experimental iOS VM environments:

```bash
ioslab vm new "iPhone VM" --cpu 4 --memory 6 --disk 64

ioslab vm boot <vmId>

ioslab vm backup <vmId> "Clean-Install"

ioslab vm restore <vmId> "Clean-Install"

ioslab vm switch <vmId> --cpu 8 --memory 12

ioslab vm list
```

VM functionality is experimental and may change between releases.

---

# Architecture

MobileLab is gradually moving toward a unified architecture consisting of:

```text
                    ┌───────────────────────┐
                    │      MobileLab        │
                    │  Orchestration Layer  │
                    └───────────┬───────────┘
                                │
             ┌──────────────────┼──────────────────┐
             │                  │                  │
             ▼                  ▼                  ▼
        Device Pool        Job Scheduler       REST / MCP
             │                  │                  │
       ┌─────┴─────┐            │            AI / CI / Tools
       │           │            │
      iOS       Android         │
       │           │            │
   Simulator   Emulator         │
       │           │            │
       └─────┬─────┘            │
             │                  │
             ▼                  ▼
        Resource Manager ── Test Execution
```

The architecture is intentionally designed so that platform-specific execution backends can evolve independently while sharing orchestration, scheduling, APIs, and reporting infrastructure.

---

# Migration from iOSLab

The transition from **iOSLab → MobileLab** is currently ongoing.

Existing source code may contain legacy identifiers such as:

```text
iOSLab
ioslab
IOSLAB_*
```

These are not accidental.

They represent compatibility with the project's previous architecture and tooling.

The repository will gradually migrate these identifiers as the new cross-platform architecture stabilizes.

### What is changing?

```text
Before

iOSLab
  └── iOS testing
       └── Simulator / VM


Now

MobileLab
  ├── iOS
  │    ├── Simulator
  │    └── experimental VM
  │
  └── Android
       └── development / testing
```

The goal is **not** to discard the existing iOSLab implementation, but to evolve it into a broader mobile testing platform.

---

# Roadmap

### Current

* [x] Parallel iOS Simulator execution
* [x] Device/OS matrix testing
* [x] Resource-aware scheduling
* [x] REST API
* [x] MCP integration
* [x] Local CI workflows
* [x] Experimental iOS virtualization
* [x] Visual regression infrastructure
* [ ] Android execution backend
* [ ] Android device orchestration
* [ ] Unified iOS/Android matrix
* [ ] ARM-focused Android testing

### Future

* [ ] Multi-Mac clustering
* [ ] Distributed execution
* [ ] Physical device integration
* [ ] Cross-platform visual regression
* [ ] Unified Android/iOS device pools
* [ ] Remote execution nodes
* [ ] Expanded ARM infrastructure
* [ ] AI-generated mobile test flows

---

# Contributing

Contributions are welcome.

Areas where contributions are particularly useful include:

* scheduling and resource management
* simulator/emulator orchestration
* Android support
* ARM infrastructure
* distributed execution
* testing infrastructure
* developer experience
* documentation

For major architectural changes, please open an issue before submitting a large pull request.

---

# Project Status

MobileLab is an actively evolving open-source project.

The iOS side is currently the most mature part of the platform, while Android and cross-platform execution are being developed and tested.

Some functionality described in this README represents **experimental or in-development architecture** rather than a stable public API.

Interfaces, commands, and implementation details may therefore change as the project transitions from its original iOSLab architecture to the broader MobileLab platform.

---

# License

This project is licensed under the **MIT License**.

See [`LICENSE.md`](LICENSE.md) for details.

---

## Formerly iOSLab

**MobileLab was formerly known as iOSLab.**

The original iOSLab project focused on parallel iOS Simulator testing on macOS. The project is now expanding toward a cross-platform mobile testing and device orchestration platform covering **iOS, Android, and ARM-based development environments**.

Existing code and historical artifacts may continue to reference `iOSLab` during this transition.
