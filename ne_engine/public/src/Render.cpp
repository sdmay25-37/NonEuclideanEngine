#include "Render.h"

#include <glad/glad.h>

Render::Render() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &UV_VBO);
	glGenBuffers(1, &MODEL_MAT_VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VERTEX BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// vertex texture coords attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// buffer vertex data
	std::vector<Vertex> vertices = {
		Vertex {  0.5,  0.5, 0.0, 1.0, 1.0 },
		Vertex {  0.5, -0.5, 0.0, 1.0, 0.0 },
		Vertex { -0.5, -0.5, 0.0, 0.0, 0.0 },
		Vertex { -0.5,  0.5, 0.0, 0.0, 1.0 }
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// UV BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);

	// sprite texture coords attribute
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	// MODEL MATRIX BUFFER //
	glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);

	// model matrix attribute
	for(int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(3 + i);
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(3 + i, 1);
	}

	// INDEX BUFFER //
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// buffer index data
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

Render::~Render() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UV_VBO);
	glDeleteBuffers(1, &MODEL_MAT_VBO);
	glDeleteBuffers(1, &EBO);
}

void Render::render(entt::registry &registry) {
	auto view = registry.view<Sprite>();

	std::vector<glm::mat4> model_mats;
	std::vector<glm::vec4> uv_ranges;

	for(auto [entity, sprite]: view.each()) {
		model_mats.push_back(sprite.model_mat);
		uv_ranges.emplace_back(sprite.uv_min.x, sprite.uv_min.y, sprite.uv_max.x, sprite.uv_max.y);
	}

	glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_mats.size(), model_mats.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

	glDrawElementsInstanced(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, nullptr, view.size());
}

void Render::bind() {
	glBindVertexArray(VAO);
}