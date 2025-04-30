
#include <iostream>

#include "App.hpp"
#include "ne_plugin/DefaultPlugins.hpp"
#include "ne_plugin/window/GLFWWindow.hpp"

void LoadTextures(Resource<TextureManager> texture_manager) {
	texture_manager->LoadTextures("../res/textures");
}

void Hello1() {
	std::cout << "1\n";
}

void Hello2() {
	std::cout << "2\n";
}


int main() {
	int screen_width = 1080;
	int screen_height = 1080;

	glm::vec3 camera_pos(0.0, 0.0, 2.0);
	glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::mat4 proj_mat = glm::ortho(0.0f, (float)screen_width, 0.0f, (float)screen_height, nearPlane, farPlane);

	App()
		.InsertResourceBase<Window, GLFWWindow>(screen_width, screen_height)
		.InsertResource<Camera>(camera_pos, camera_up, proj_mat)
		.AddPlugin<DefaultPlugins>()
		.AddSystems(ScheduleLabel::STARTUP, std::move(SystemSet(Hello1).Then(Hello2)))
		.Run();
}
