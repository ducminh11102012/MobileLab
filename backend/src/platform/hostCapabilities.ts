import os from "node:os";
import { access } from "node:fs/promises";

export type HostCapabilities = {
  platform: NodeJS.Platform;
  architecture: string;
  kernel: string;
  node: string;
  arm64Linux: boolean;
  kvmDevice: boolean;
  supportedTargets: string[];
};

export async function getHostCapabilities(): Promise<HostCapabilities> {
  const platform = os.platform();
  const architecture = process.arch;
  const arm64Linux = platform === "linux" && architecture === "arm64";

  let kvmDevice = false;
  if (platform === "linux") {
    try {
      await access("/dev/kvm");
      kvmDevice = true;
    } catch {
      kvmDevice = false;
    }
  }

  return {
    platform,
    architecture,
    kernel: os.release(),
    node: process.version,
    arm64Linux,
    kvmDevice,
    supportedTargets: arm64Linux ? ["linux-arm64-native"] : [],
  };
}
