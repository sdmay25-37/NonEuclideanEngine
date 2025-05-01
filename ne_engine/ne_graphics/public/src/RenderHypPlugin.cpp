
#include "RenderHypPlugin.hpp"

void RenderHypPlugin::Setup(Resource<RendererHyp> renderer) {
	renderer->Init();
}

void RenderHypPlugin::Render(entt::registry &registry, Resource<RendererHyp> renderer, Resource<Camera> camera, Resource<Window> window) {
	renderer->Render(registry, camera);
	window->SwapBuffers();
}