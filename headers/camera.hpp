#pragma once

namespace Camera {
	float zoom = 1.0f;
	Vec2 position(0.0f);
	glm::mat4 viewMatrix;

	unsigned int width;
	unsigned int height;
	float aspectRatio;
	glm::mat4 projectionMatrix;



	inline void updateViewMatrix() {
		viewMatrix = glm::mat4(1.0f);
		viewMatrix[0][0] = zoom;
		viewMatrix[1][1] = zoom;
		viewMatrix = translate(viewMatrix, glm::vec3(position.x, position.y, 0));
	}

	inline void updateProjectionMatrix(unsigned int width, unsigned int height) {
		Camera::width = width;
		Camera::height = height;
		aspectRatio = (float)width / height;

		projectionMatrix = glm::mat4(1.0f);
		if (aspectRatio > 1) {
			projectionMatrix[0][0] = 1 / aspectRatio;
		} else if (aspectRatio < 1) {
			projectionMatrix[1][1] = aspectRatio;
		}
	}
}
