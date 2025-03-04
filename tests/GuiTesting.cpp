#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <IconsFontAwesome5.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "ui/tools/AnimationEditor.hpp"
#include "ui/tools/KeyMappingsManager.hpp"
#include "ui/tools/TextureAtlasBuilder.hpp"


void glfw_error_callback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << std::endl;
}

int main() {
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
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

	io.Fonts->AddFontDefault();
	constexpr float base_font_size = 16.0f;
	constexpr float icon_font_size = base_font_size * 2.0f / 3.0f;

	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icon_config;
	icon_config.MergeMode = true;
	icon_config.PixelSnapH = true;
	icon_config.GlyphMinAdvanceX = icon_font_size;

	io.Fonts->AddFontFromFileTTF("../res/fonts/fa-solid-900.ttf", icon_font_size, &icon_config, icon_ranges);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	std::unique_ptr<AnimationEditor> animation_editor;
	std::unique_ptr<KeyMappingsManager> key_mappings_manager;
	std::unique_ptr<TextureAtlasBuilder> texture_atlas_builder;


	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();
		static bool init = true;
		if(init) {
			init = false;
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear previous layout
			ImGuiID dock_id_main = ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dock_id_main, ImGui::GetMainViewport()->Size);

			ImGui::DockBuilderDockWindow(TextureAtlasBuilder::GUI_ID, dock_id_main);
		}


		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu(ICON_FA_FILE " File")) {
				if(ImGui::MenuItem(ICON_FA_POWER_OFF " Exit")) {
					glfwSetWindowShouldClose(window, true);
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu(ICON_FA_WRENCH " Tools")) {
				if(ImGui::MenuItem(AnimationEditor::GUI_NAME)) {
					animation_editor = std::make_unique<AnimationEditor>();
				}

				if(ImGui::MenuItem(KeyMappingsManager::GUI_NAME)) {
					key_mappings_manager = std::make_unique<KeyMappingsManager>();
				}

				if(ImGui::MenuItem(TextureAtlasBuilder::GUI_NAME)) {
					texture_atlas_builder = std::make_unique<TextureAtlasBuilder>();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if(animation_editor != nullptr) {
			animation_editor->render();

		} else if(key_mappings_manager != nullptr) {
			key_mappings_manager->render();

		} else if(texture_atlas_builder != nullptr) {
			texture_atlas_builder->render();

		} else {
			ImGui::ShowDemoWindow();
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