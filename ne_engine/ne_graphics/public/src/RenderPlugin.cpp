
#include "RenderPlugin.hpp"

void RenderPlugin::Setup(Resource<Renderer> renderer) {
	renderer->init();
}

void RenderPlugin::Render(entt::registry& registry, Resource<Renderer> renderer) {
	renderer->render(registry);
}