#include "Renderer.hpp"

#include <iostream>
#include <TextureManager.hpp>
#include <glad/glad.h>
#include "ne_engine.hpp"

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VE0);
}
void Renderer::Init()
{
	// Generate all buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VE0);

	// --- Instanced Rendering Setup ---
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, x));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, color));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MeshPoint), (void *)offsetof(MeshPoint, uv));
	glEnableVertexAttribArray(2);

	// Load shaders
	auto shader_result = ShaderProgram::create(
		"../ne_engine/ne_math/shaders/pq_test.vert",
		"../ne_engine/ne_math/shaders/pq_color.frag");
	if (shader_result.is_error())
	{
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	_shader_program = std::make_unique<ShaderProgram>(shader_result.ok());
	_shader_program->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Todo: Not sure how I feel about this method
// I don't like having to copy UV data every frame when it likely doesn't change
void Renderer::Render(entt::registry &registry, Resource<Camera> camera) const
{

	HypRotate r_uniform_matrix = HypRotate(true);

	_shader_program->setUniform1i("texture_atlas", 0);

	_shader_program->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

	auto pqTile_view = registry.view<AtlasPQtile>();

	for (auto [entity, currentTile] : pqTile_view.each())
	{
		currentTile.tile.to_weirstrass(); // Ensures Poincaré conversion
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MeshPoint) * currentTile.tile.mesh_size(), currentTile.tile.mesh_data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VE0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * currentTile.tile.indices_size(), currentTile.tile.indices_data(), GL_STATIC_DRAW);

		glDrawElements(GL_TRIANGLES, currentTile.tile.indices_size(), GL_UNSIGNED_INT, 0);
	}
}

void Renderer::Bind()
{
	glBindVertexArray(VAO);
	_shader_program->bind();
}