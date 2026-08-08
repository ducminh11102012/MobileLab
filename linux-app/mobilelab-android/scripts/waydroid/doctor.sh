#!/usr/bin/env bash
set -euo pipefail
printf 'MobileLab Android / Waydroid doctor\n'
printf 'arch: '; uname -m
command -v waydroid >/dev/null && echo 'waydroid: available' || echo 'waydroid: missing'
command -v adb >/dev/null && echo 'adb: available' || echo 'adb: missing'
[ -e /dev/binder ] && echo 'binder: available' || echo 'binder: unavailable'
[ -e /dev/kvm ] && echo 'kvm: available' || echo 'kvm: unavailable'
