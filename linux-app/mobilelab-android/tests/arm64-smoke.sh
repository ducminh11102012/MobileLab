#!/usr/bin/env bash
set -euo pipefail

fail(){ echo "[FAIL] $*" >&2; exit 1; }

ARCH="$(uname -m)"
case "$ARCH" in
  aarch64|arm64) echo "[PASS] host architecture: $ARCH" ;;
  *) echo "[INFO] host architecture: $ARCH (ARM64 execution is validated on aarch64 hosts)" ;;
esac

command -v cmake >/dev/null || fail "cmake is required"
command -v adb >/dev/null && echo "[PASS] adb detected" || echo "[WARN] adb not installed"
command -v emulator >/dev/null && echo "[PASS] Android emulator detected" || echo "[WARN] Android emulator not installed"

if [ -e /dev/kvm ]; then echo "[PASS] KVM available"; else echo "[INFO] KVM unavailable; userspace ARM path expected"; fi

SDK="${ANDROID_SDK_ROOT:-${ANDROID_HOME:-$HOME/Android/Sdk}}"
if [ -d "$SDK/system-images" ]; then
  count=$(find "$SDK/system-images" -mindepth 3 -maxdepth 3 -type d 2>/dev/null | wc -l)
  echo "[PASS] SDK system-image tree present ($count entries)"
else
  echo "[INFO] no Android system-image tree at $SDK/system-images"
fi

echo "[PASS] MobileLab ARM64 environment smoke test completed"
