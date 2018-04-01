#include "camera.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace camera {
	float zoom = 1.0f;
	vec2 position(0.0f);
	glm::mat4 view_matrix;

	unsigned int width;
	unsigned int height;
	float aspect_ratio;
	glm::mat4 projection_matrix;



	void update_view_matrix() {
		view_matrix = glm::mat4(1.0f);
		view_matrix[0][0] = zoom;
		view_matrix[1][1] = zoom;
		view_matrix = translate(view_matrix, glm::vec3(position.x, position.y, 0));
	}

	void update_projection_matrix(unsigned int width, unsigned int height) {
		camera::width = width;
		camera::height = height;
		aspect_ratio = (float)width / height;

		projection_matrix = glm::mat4(1.0f);
		if (aspect_ratio > 1) {
			projection_matrix[0][0] = 1 / aspect_ratio;
		}
		else if (aspect_ratio < 1) {
			projection_matrix[1][1] = aspect_ratio;
		}
	}
}
