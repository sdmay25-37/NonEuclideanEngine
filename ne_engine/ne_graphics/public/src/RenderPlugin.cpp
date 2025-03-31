
#include "RenderPlugin.hpp"

void RenderPlugin::Setup(Resource<Renderer> renderer) {
	renderer->Init();
}

void RenderPlugin::Render(entt::registry& registry, Resource<Renderer> renderer) {
	renderer->Render(registry);
}