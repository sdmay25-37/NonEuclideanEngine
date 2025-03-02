#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>
#include <stb_image.h>
#include <filesystem>
#include <imgui_internal.h>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

#include "Texture.hpp"
#include "ui/components/Image.hpp"
#include "ui/tools/TextureAtlasBuilder.hpp"
#include "utils.hpp"


void glfw_error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Gui Testing", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	bool atlas_tool_window = false;
	bool gui_init = true;

	TextureAtlasBuilder texture_atlas_builder("../res/textures");

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport();

		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("Exit")) {
					glfwSetWindowShouldClose(window, true);
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Tools")) {
				if(ImGui::MenuItem("Texture Atlas Builder")) {
					atlas_tool_window = true;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if(atlas_tool_window) {
			texture_atlas_builder.render();
		}

		ImGui::Render();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}