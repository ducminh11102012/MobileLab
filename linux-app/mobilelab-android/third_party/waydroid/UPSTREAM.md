# Waydroid integration

MobileLab uses the upstream Waydroid project as an optional Android container backend rather than copying its source tree into this repository.

Upstream project: https://github.com/waydroid/waydroid
Upstream license: GPL-3.0

The integration layer under `src/waydroid/` invokes the installed Waydroid runtime and translates its lifecycle into MobileLab's device/runtime abstraction. Android system images remain external runtime assets and are not vendored into MobileLab.

This keeps upstream provenance explicit, avoids duplicating the upstream repository, and allows MobileLab to track Waydroid independently.
