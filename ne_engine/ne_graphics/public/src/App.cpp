#include <ctime>
#include <iostream>
#include <thread>


#include <stb_image.h>

#include "App.hpp"

#include <Renderer.hpp>
#include <Window.hpp>

#include "JSONLoader.hpp"

#include <glad/glad.h>


constexpr int UPDATES_PER_SECOND = 60;
constexpr double SECONDS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
constexpr int MAX_FRAMESKIP = 5;


void App::Run() {

	Init();
	glfwMakeContextCurrent(NULL);
	auto window = _resource_manager.get<Window>();

	Synchronizer frameSynch(2);
	std::thread render_thread([&] {
		glfwMakeContextCurrent(window->ptr);
		while(!glfwWindowShouldClose(window->ptr)) {
			Render();
			frameSynch.wait();
		}

		// TODO: this is a temporary fix as the main thread still makes some OpenGL calls
		glfwMakeContextCurrent(NULL);
	});


	while(!glfwWindowShouldClose(window->ptr)) {

		Update();
		frameSynch.wait();

	}

	render_thread.join();

	glfwMakeContextCurrent(window->ptr);
	Cleanup();
}

void App::Init() {

	_executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded, _registry, _resource_manager);
	_executor->Execute(_schedules[ScheduleLabel::PRE_STARTUP]);

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("../res/textures/tiles.png", &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// build and compile our shader program
	auto shader_result = ShaderProgram::create(
			"../shaders/sprite.vert",
			"../shaders/sprite.frag"
	);

	if(shader_result.is_error()) {
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	_shaders = new ShaderProgram(shader_result.ok());

	auto texture_manager = _resource_manager.get<TextureManager>();

	auto result = texture_manager->loadAtlas("../res/atlases/atlas.json");
	if(result.is_error()) {
		std::cerr << "Error: " << result.error();
	}

	_executor->Execute(_schedules[ScheduleLabel::STARTUP]);

	auto renderer = _resource_manager.get<Renderer>();
	renderer->bind();

	_shaders->bind();
	_shaders->setUniform1i("texture_atlas", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	auto window = _resource_manager.get<Window>();
	_charInput = new Input(window->ptr);
	JSONLoader loader("../tests/bindings/example_bindings.json");

	auto bindings = loader.processFileArray();

	_charInput->bindKeyPress("QUIT", [this, window]() {
		glfwSetWindowShouldClose(window->ptr, true);
	});

	_charInput->bindContexts(bindings);
}

void App::Update() {
	glfwPollEvents();
	_executor->Execute(_schedules[ScheduleLabel::UPDATE]);
}

void App::Render() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glm::vec3 camera_pos(0.0, 0.0, 1.0);
	glm::vec4 camera_up(0.0, 1.0, 0.0, 1.0);

	float fov = glm::radians(45.0f);
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::mat4 proj_mat = glm::perspective(fov, ASPECT_RATIO, nearPlane, farPlane);

    glm::mat4 proj_view_mat = proj_mat * glm::lookAt(camera_pos, glm::vec3(camera_pos.x, camera_pos.y, 0.0), xyz(camera_up));
    _shaders->setUniformMat4("proj_view_mat", proj_view_mat);

	_executor->Execute(_schedules[ScheduleLabel::RENDER]);

	auto window = _resource_manager.get<Window>();
    glfwSwapBuffers(window->ptr);
}

void App::Cleanup() {
	delete _charInput;
	_charInput = nullptr;

	_shaders->cleanup();
	delete _shaders;
	_shaders = nullptr;
}

