#include <DraggableCanvas.hpp>
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

#include "Image.hpp"
#include "Texture.hpp"
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

	std::vector<Image> images;
	DraggableCanvas canvas;

	try {
		for(const auto &entry : std::filesystem::directory_iterator("../res/textures")) {
			if(is_regular_file(entry.status())) {
				const char *filepath = entry.path().c_str();
				std::string filename = entry.path().filename().string();

				auto texture_result = Texture::create(filepath);

				if(texture_result.is_ok()) {
					int image_x = Random::integer(0, 500);
					int image_y = Random::integer(0, 500);

					Texture texture = std::move(texture_result.ok());
					int width = texture.getWidth();
					int height = texture.getHeight();

					canvas.addComponent(std::make_unique<Image>(filename, std::move(texture), ImVec2(image_x, image_y), ImVec2(width, height)));
				} else {
					std::cerr << "Failed to load image file: " << filepath << std::endl;
				}
			}
		}
	} catch(const std::exception &e) {
		std::cerr << "Failed to load file: " << e.what() << std::endl;
	}

	bool atlas_tool_window = false;
	bool gui_init = true;

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();

		// if(gui_init) {
		// 	ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;
		// 	ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
		// 	ImVec2 workCenter = ImGui::GetMainViewport()->GetWorkCenter();
		//
		// 	ImGuiID id = ImGui::GetID("MainWindowGroup");
		// 	ImGui::DockBuilderRemoveNode(id);
		// 	ImGui::DockBuilderAddNode(id);
		//
		// 	ImVec2 size{ 600, 300 };
		// 	ImVec2 nodePos{ workCenter.x - size.x * 0.5f, workCenter.y - size.y * 0.5f };
		//
		// 	ImGui::DockBuilderSetNodeSize(id, size);
		// 	ImGui::DockBuilderSetNodePos(id, nodePos);
		//
		// 	ImGuiID dock1 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.5f, nullptr, &id);
		// 	ImGuiID dock2 = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.5f, nullptr, &id);
		// 	ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down, 0.5f, nullptr, &dock2);
		//
		// 	ImGui::DockBuilderDockWindow("One", dock1);
		// 	ImGui::DockBuilderDockWindow("Two", dock2);
		// 	ImGui::DockBuilderDockWindow("Three", dock3);
		//
		// 	ImGui::DockBuilderFinish(id);
		//
		// 	gui_init = false;
		// }

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
			ImGui::Begin("Texture Atlas Builder", nullptr);

			static bool init = true;
			if(init) {
				init = false;

				ImVec2 size = ImGui::GetContentRegionAvail();

				ImVec2 canvasSize = size;
				canvasSize.x *= 0.8;
				canvasSize.y *= 0.8;
				canvas.setSize(canvasSize);


				canvas.setPosition(ImVec2((size.x - canvasSize.x) / 2, (size.y - canvasSize.y) / 2));
			}


			canvas.render();

			ImGui::End();
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