import test from "node:test";
import assert from "node:assert/strict";
import { getHostCapabilities } from "../../src/platform/hostCapabilities";

test("reports host architecture and native ARM64 capability", async () => {
  const capabilities = await getHostCapabilities();

  assert.equal(typeof capabilities.architecture, "string");
  assert.equal(typeof capabilities.platform, "string");
  assert.equal(typeof capabilities.arm64Linux, "boolean");
  assert.equal(typeof capabilities.kvmDevice, "boolean");
  assert.ok(Array.isArray(capabilities.supportedTargets));

  if (capabilities.arm64Linux) {
    assert.ok(capabilities.supportedTargets.includes("linux-arm64-native"));
  }
});
