#!/usr/bin/env bash
set -euo pipefail

# MobileLab intentionally does not vendor Waydroid. This helper checks the
# host and prepares an installed upstream Waydroid runtime for integration.

command -v waydroid >/dev/null || { echo "waydroid is not installed" >&2; exit 1; }
command -v adb >/dev/null || { echo "adb is not installed" >&2; exit 1; }

arch="$(uname -m)"
echo "MobileLab Android backend: host architecture=${arch}"
waydroid status || true
waydroid prop get ro.product.cpu.abi || true

echo "Waydroid integration probe complete."
