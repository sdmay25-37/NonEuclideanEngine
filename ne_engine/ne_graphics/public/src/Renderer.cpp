#include "Renderer.hpp"

#include <iostream>
#include <TextureManager.hpp>
#include <glad/glad.h>
#include "ne_engine.hpp"

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &UV_VBO);
	glDeleteBuffers(1, &MODEL_MAT_VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VE0);
	glDeleteVertexArrays(1, &mesh_VAO);
	glDeleteBuffers(1, &mesh_VBO);
	glDeleteBuffers(1, &mesh_EBO);
}
void Renderer::Init()
{
	// Generate all buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &UV_VBO);
	glGenBuffers(1, &MODEL_MAT_VBO);
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

	glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);
	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(3 + i);
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(3 + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error before rendering: " << error << std::endl;
	}
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void *)0);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7, 1);

	PQTile tile(4, 5, COLOR::BLUE); // placeholder mesh to fill the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tile.indices_size(), tile.indices_data(), GL_STATIC_DRAW);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error before rendering: " << error << std::endl;
	}
	// --- Mesh Rendering Setup (non-instanced) ---
	glGenVertexArrays(1, &mesh_VAO);
	glGenBuffers(1, &mesh_VBO);
	glGenBuffers(1, &mesh_EBO);

	// Bind VAO first
	glBindVertexArray(mesh_VAO);

	// Bind and configure vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO);
	// We don’t upload data yet here — that happens per frame in Render()

	// Bind and configure element buffer (EBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_EBO);
	// We don’t upload index data yet here either — that happens in Render()

	// Define vertex attribute layout:
	// Assuming interleaved layout: vec4 position (x, y, z, w), vec3 color (r, g, b), vec2 uv (u, v)
	// = 9 floats per vertex, total stride = 9 * sizeof(float)
	GLsizei stride = sizeof(float) * 9;

	// Position (x, y, z, w) — location 0
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride, (void *)0);
	glEnableVertexAttribArray(0);

	// Color (r, g, b) — location 1
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 4));
	glEnableVertexAttribArray(1);

	// UV (u, v) — location 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *)(sizeof(float) * 7));
	glEnableVertexAttribArray(2);

	// Optional: check for OpenGL errors
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error during mesh VAO setup: " << error << std::endl;
	}
	// Load shaders
	auto shader_result = ShaderProgram::create(
		"../ne_engine/ne_math/shaders/pq_test.vert",
		"../ne_engine/ne_math/shaders/pq_color.frag");
	if (shader_result.is_error())
	{
		std::cerr << "Failed to create shader program: " << shader_result.error() << std::endl;
		return;
	}

	HypRotate r_uniform_matrix = HypRotate(true);

	_shader_program = std::make_unique<ShaderProgram>(shader_result.ok());
	_shader_program->setUniform1i("texture_atlas", 0);

	_shader_program->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

	_shader_program->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Todo: Not sure how I feel about this method
// I don't like having to copy UV data every frame when it likely doesn't change
void Renderer::Render(entt::registry &registry, Resource<Camera> camera) const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error before rendering: " << error << std::endl;
	}

	HypRotate r_uniform_matrix = HypRotate(true);

	_shader_program->setUniform1i("texture_atlas", 0);

	_shader_program->setUniformMat4("r_matrix", r_uniform_matrix.getRotation());

	_shader_program->bind();
	// // --- Instanced Sprite Rendering ---
	// auto sprite_view = registry.view<AtlasSprite>();

	// std::unordered_map<unsigned int, std::pair<std::vector<glm::mat4>, std::vector<glm::vec4>>> atlas_data;

	// for (auto [entity, sprite] : sprite_view.each())
	// {
	// 	auto &[model_mats, uv_ranges] = atlas_data[sprite.texture.atlas_id];
	// 	model_mats.push_back(sprite.model_mat);
	// 	uv_ranges.emplace_back(sprite.texture.uv_min.x, sprite.texture.uv_min.y,
	// 						   sprite.texture.uv_max.x, sprite.texture.uv_max.y);
	// }

	// glBindVertexArray(VAO);

	// for (const auto &[atlas_id, val] : atlas_data)
	// {
	// 	const auto &[model_mats, uv_ranges] = val;

	// 	glBindBuffer(GL_ARRAY_BUFFER, MODEL_MAT_VBO);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * model_mats.size(), model_mats.data(), GL_DYNAMIC_DRAW);

	// 	glBindBuffer(GL_ARRAY_BUFFER, UV_VBO);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * uv_ranges.size(), uv_ranges.data(), GL_STATIC_DRAW);

	// 	glActiveTexture(GL_TEXTURE0);
	// 	glBindTexture(GL_TEXTURE_2D, atlas_id);

	// 	glDrawElementsInstanced(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, nullptr, model_mats.size());
	// }

	// --- Mesh Rendering (non-instanced) ---
	auto mesh_view = registry.view<AtlasMesh>();

	glBindVertexArray(mesh_VAO);
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cerr << "OpenGL error before rendering: " << error << std::endl;
	}

	for (auto [entity, mesh] : mesh_view.each())
	{
		std::vector<float> interleaved;
		interleaved.reserve(mesh.positions.size() * 8); // Adjusted for position, color, UVs

		if (mesh.texture_id == 0)
		{
			std::cerr << "Texture not loaded properly!" << std::endl;
		}
		else
		{
		}

		// Iterate through the mesh and interleave position, color, and uv data
		for (size_t i = 0; i < mesh.positions.size(); ++i)
		{
			const auto &position = mesh.positions[i]; // Access position data
			const auto &color = mesh.colors[i];		  // Access color data
			const auto &uv = mesh.uvs[i];			  // Access UV data directly

			// Interleave the data: position (x, y, z, w), color (r, g, b), UV (u, v)
			interleaved.push_back(position.x);
			interleaved.push_back(position.y);
			interleaved.push_back(position.z);
			interleaved.push_back(position.w); // Assuming positions are glm::vec4
			interleaved.push_back(color.r);
			interleaved.push_back(color.g);
			interleaved.push_back(color.b);
			interleaved.push_back(color.a);
			interleaved.push_back(uv.x);
			interleaved.push_back(uv.y);
		}

		// Bind the VBO (Vertex Buffer Object) and upload the interleaved data
		glBindBuffer(GL_ARRAY_BUFFER, mesh_VBO);
		glBufferData(GL_ARRAY_BUFFER, interleaved.size() * sizeof(float), interleaved.data(), GL_DYNAMIC_DRAW);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error before rendering: " << error << std::endl;
		}
		// Bind the EBO (Element Buffer Object) and upload the indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_DYNAMIC_DRAW);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error before rendering: " << error << std::endl;
		}
		// Activate texture unit 0 and bind the texture atlas using texture_id

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.texture_id);
		glGenerateMipmap(GL_TEXTURE_2D);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error before rendering: " << error << std::endl;
		}

		// Draw the mesh elements
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cerr << "OpenGL error before rendering: " << error << std::endl;
		}
	}
}

void Renderer::Bind()
{
	glBindVertexArray(VAO);
	_shader_program->bind();
}