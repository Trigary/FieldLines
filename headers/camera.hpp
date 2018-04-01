#pragma once
#include "glm/glm.hpp"
#include "vector.hpp"

namespace camera {
	extern float zoom;
	extern vec2 position;
	extern glm::mat4 view_matrix;

	extern unsigned int width;
	extern unsigned int height;
	extern float aspect_ratio;
	extern glm::mat4 projection_matrix;

	void update_view_matrix();
	void update_projection_matrix(unsigned int width, unsigned int height);
}
