#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
SDK_ROOT="${ANDROID_SDK_ROOT:-${ANDROID_HOME:-$HOME/Android/Sdk}}"
CMDLINE_TOOLS="$SDK_ROOT/cmdline-tools/latest/bin"

mkdir -p "$SDK_ROOT" "$ROOT/runtime/android-sdk" "$ROOT/runtime/avd" "$ROOT/runtime/artifacts"

echo "MobileLab Android ARM64 bootstrap"
echo "SDK root: $SDK_ROOT"

if command -v sdkmanager >/dev/null 2>&1; then
  SDKMANAGER="$(command -v sdkmanager)"
elif [[ -x "$CMDLINE_TOOLS/sdkmanager" ]]; then
  SDKMANAGER="$CMDLINE_TOOLS/sdkmanager"
else
  echo "Android SDK command-line tools are not installed."
  echo "Install the official Android SDK Command-line Tools, then rerun this script."
  exit 2
fi

export ANDROID_SDK_ROOT="$SDK_ROOT"
"$SDKMANAGER" --sdk_root="$SDK_ROOT" "platform-tools" "emulator" "platforms;android-35" "system-images;android-35;google_apis;arm64-v8a"

printf '%s\n' "Bootstrap complete." "Use tools/android/create-avd.sh to create a MobileLab ARM64 AVD."
