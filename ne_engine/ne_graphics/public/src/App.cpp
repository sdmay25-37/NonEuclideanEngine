#include <ctime>
#include <iostream>

#include <glad/glad.h>
#include <stb_image.h>

#include "App.hpp"
#include "JSONLoader.hpp"


constexpr int UPDATES_PER_SECOND = 60;
constexpr double SECONDS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
constexpr int MAX_FRAMESKIP = 5;


void App::run() {

	init();

	// TESTING
	unsigned long updates = 0;
	double startTime = glfwGetTime();

	double last_time = glfwGetTime();
	double acc = 0.0;
	int frames_skipped = 0;

	while (!glfwWindowShouldClose(_window)) {
		double current_time = glfwGetTime();
		double delta_time = current_time - last_time;

		last_time = current_time;
		acc += delta_time;

		while(acc > SECONDS_PER_UPDATE && frames_skipped < MAX_FRAMESKIP) {
			update();
			updates++;
			acc -= SECONDS_PER_UPDATE;
			frames_skipped++;
		}

		// Only render if at least one update has occurred
		if(frames_skipped > 0) {
			render();
			frames_skipped = 0;
			// std::cout << "UPS: " << updates / (current_time - startTime) << std::endl;
		}

	}

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

	std::srand(std::time(nullptr));

	_tileWidth = width / 16;
	_tileHeight = height / 16;

	_uvWidth = 1.0 / _tileWidth;
	_uvHeight = 1.0 / _tileHeight;

	int map_width = 20;
	int map_height = map_width * ASPECT_RATIO;

	float rect_size = 2.0 / (map_width - 1);
	int num_sprites = map_width * map_height;

	auto result = _texture_manager.loadAtlas("../res/atlases/atlas.json");
	if(result.is_error()) {
		std::cerr << "Error: " << result.error();
	}

	auto texture_result = _texture_manager.getTexture("tile0.png");
	AtlasedTexture texture = texture_result.value();

	for(int i = 0; i < num_sprites; i++) {

		int x = i % map_width;
		int y = i / map_width;

		float rect_x = x * rect_size - 1.0;
		float rect_y = y * rect_size - 1.0;

		int tile_x = rand() % _tileWidth;
		int tile_y = rand() % _tileHeight;

		float u = tile_x * _uvWidth;
		float v = tile_y * _uvHeight;

		glm::vec3 position(rect_x, rect_y, 0.0);
		glm::vec3 scale(rect_size, rect_size, 1.0);
		glm::vec2 uv_min(u, v);
		glm::vec2 uv_max(u + _uvWidth, v + _uvHeight);

		glm::mat4 model_mat(1.0);
		model_mat = glm::translate(model_mat, position);
		model_mat = glm::scale(model_mat, scale);

		_uvRanges.emplace_back(uv_min.x, uv_min.y, uv_max.x, uv_max.y);

		const auto entity = _registry.create();
		// _registry.emplace<Sprite>(entity, model_mat, uv_min, uv_max);
		_registry.emplace<AtlasSprite>(entity, model_mat, texture);
	}

	_renderSystem = new Render();
	_renderSystem->bind();

	_shaders->bind();
	_shaders->setUniform1i("texture_atlas", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	_charInput = new Input(_window);
	JSONLoader loader("../tests/bindings/example_bindings.json");

	auto bindings = loader.processFileArray();

	_charInput->bindKeyPress("QUIT", [this]() {
		glfwSetWindowShouldClose(_window, true);
	});

	_charInput->bindContexts(bindings);
}

void App::update() {
	glfwPollEvents();
	_count++;

	if(_count % 25 == 0) {
		_uvRanges.clear();

		auto view = _registry.view<Sprite>();
		for(auto [entity, sprite] : view.each()) {
			int tile_x = rand() % _tileWidth;
			int tile_y = rand() % _tileHeight;

			float u = tile_x * _uvWidth;
			float v = tile_y * _uvHeight;

			sprite.uv_min = glm::vec2(u, v);
			sprite.uv_max = glm::vec2(u + _uvWidth, v + _uvHeight);
		}
	}

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

	glfwTerminate();
}

