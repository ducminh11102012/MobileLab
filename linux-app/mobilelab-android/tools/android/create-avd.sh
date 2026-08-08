#!/usr/bin/env bash
set -euo pipefail

NAME="mobilelab-arm64-api35"
API="35"
ABI="arm64-v8a"
DEVICE="pixel_9"
SDK_ROOT="${ANDROID_SDK_ROOT:-${ANDROID_HOME:-$HOME/Android/Sdk}}"

while [[ $# -gt 0 ]]; do
  case "$1" in
    --name) NAME="$2"; shift 2;;
    --api) API="$2"; shift 2;;
    --abi) ABI="$2"; shift 2;;
    --device) DEVICE="$2"; shift 2;;
    *) echo "Unknown option: $1"; exit 2;;
  esac
done

AVDMANAGER="$SDK_ROOT/cmdline-tools/latest/bin/avdmanager"
[[ -x "$AVDMANAGER" ]] || AVDMANAGER="$(command -v avdmanager || true)"
[[ -n "$AVDMANAGER" ]] || { echo "avdmanager not found"; exit 2; }

IMAGE="system-images;android-${API};google_apis;${ABI}"
echo "Creating ARM64 AVD: $NAME"
echo "Image: $IMAGE"

tmp=$(mktemp)
printf 'no\n' | "$AVDMANAGER" create avd -n "$NAME" -k "$IMAGE" -d "$DEVICE" --force
rm -f "$tmp"

echo "AVD created: $NAME"
echo "Start it through MobileLab or: emulator -avd $NAME"
