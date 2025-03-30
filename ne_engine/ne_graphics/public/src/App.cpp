#include <ctime>
#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <stb_image.h>

#include "App.hpp"
#include "JSONLoader.hpp"


constexpr int UPDATES_PER_SECOND = 60;
constexpr double SECONDS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
constexpr int MAX_FRAMESKIP = 5;


void App::run() {

	for(auto& plugin : _plugins) {
		plugin->build(*this);
	}

	init();
	glfwMakeContextCurrent(NULL);

	Synchronizer frameSynch(2);

	std::thread render_thread([&] {
		glfwMakeContextCurrent(_window);
		while(!glfwWindowShouldClose(_window)) {
			render();
			frameSynch.wait();
		}

		// TODO: this is a temporary fix as the main thread still makes some OpenGL calls
		glfwMakeContextCurrent(NULL);
	});


	while(!glfwWindowShouldClose(_window)) {

		update();
		frameSynch.wait();

	}

	render_thread.join();

	glfwMakeContextCurrent(_window);
	cleanup();
}

void App::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (_window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, [] (GLFWwindow* _, const int w, const int h) {
		glViewport(0, 0, w, h);
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

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

	_renderSystem = new Render();
	_renderSystem->init();
	_renderSystem->bind();

	_shaders->bind();
	_shaders->setUniform1i("texture_atlas", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	_charInput = new Input(_window);
	JSONLoader loader("../tests/bindings/example_bindings.json");

	auto bindings = loader.processFileArray();

	_charInput->bindKeyPress("QUIT", [this]() {
		glfwSetWindowShouldClose(_window, true);
	});

	_charInput->bindContexts(bindings);

	_executor = SystemExecutor::Create(SystemExecutor::Type::SingleThreaded, _registry, _resource_manager);
	_executor->Execute(_schedules[ScheduleLabel::STARTUP]);
}

void App::update() {
	glfwPollEvents();
	_executor->Execute(_schedules[ScheduleLabel::UPDATE]);
}

void App::render() {
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

    _renderSystem->render(_registry);

    glfwSwapBuffers(_window);
}

void App::cleanup() {
	delete _charInput;
	_charInput = nullptr;

	_shaders->cleanup();
	delete _shaders;
	_shaders = nullptr;

	delete _renderSystem;
	_renderSystem = nullptr;
}

