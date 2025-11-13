#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& init_vertices, const std::vector<GLuint>& init_indices, const std::vector<Texture>& init_textures)
{
	Regenerate(init_vertices, init_indices, init_textures);
}

void Mesh::Regenerate(const std::vector<Vertex>& new_vertices, const std::vector<GLuint>& new_indices, const std::vector<Texture>& new_textures)
{
	vertices = new_vertices;

	if (new_indices.size() > 0)
		indices = new_indices;

	if (new_textures.size() > 0)
		textures = new_textures;

	// Create and bind the vertex array object
	VAO.Bind();

	// Bind the vertex buffer object using data provided by our vertices
	VBO VBO(vertices);

	// Bind the element buffer object using data provided by our indices
	EBO EBO(indices);

	// Link the VAO to our VBO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// NOTE: 
	// 0 = position,
	// 1 = normal,
	// 2 = colour, 
	// 3 = texture coordinates

	// Unbind everything
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera, GLuint drawType)
{
	shader.Activate();
	VAO.Bind();

	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].textureUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.AssignMatrix(shader, "camMatrix");
	camera.AssignProjection(shader, "projection");
	camera.AssignView(shader, "view");

	glPointSize(5.0f);
	//glLineWidth(5.0f);
	//glDrawArrays(drawType, 0, 4);
	glDrawElements(drawType, indices.size(), GL_UNSIGNED_INT, 0);
}
