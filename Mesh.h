#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	VAO VAO;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures = std::vector<Texture>());

	void Regenerate(const std::vector<Vertex>& new_vertices, const std::vector<GLuint>& new_indices = std::vector<GLuint>(), const std::vector<Texture>& new_textures = std::vector<Texture>());

	void Draw(Shader& shader, Camera& camera, GLuint drawType = GL_TRIANGLES);
};


#endif // !MESH_CLASS_H
