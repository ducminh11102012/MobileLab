import { FastifyInstance } from "fastify";
import { getHostCapabilities } from "../../platform/hostCapabilities";

export async function registerCapabilityRoutes(app: FastifyInstance): Promise<void> {
  app.get("/capabilities", async () => getHostCapabilities());
}
