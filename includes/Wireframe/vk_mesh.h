#pragma once

#include <Wireframe/vk_types.h>
#include <vector>
#include <glm/vec3.hpp>


struct Mesh {
	//std::vector<Vertex> _vertices;

	AllocatedBuffer _vertexBuffer;

	bool load_from_obj(const char* filename);
};