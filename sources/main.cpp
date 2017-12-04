#include "includes.h"
#include "chargeData.hpp"
#include "lineData.hpp"
#include "io.hpp"
#include "camera.hpp"

bool initializeWindow();
bool initializeGlad();
void initializeLineObjects();
void initializeChargeObjects();
void windowKeyPressed(int key);
void processWindowInput();
bool isKeyPressed(int key);
void renderScreen();
void activateShader(Shader shader);
void limitFrameRate();

GLFWwindow* window;
Shader lineShader(GL_ARRAY_BUFFER);
Shader chargeShader(GL_ARRAY_BUFFER);
double lastTimeStamp = 0;
float deltaTime = 0.0f;
bool viewMatrixChanged = true;
bool projectionMatrixChanged = true;



int main() {
	srand((unsigned int)time(nullptr));
	IO::printInfo();
	IO::initializeCharges();

	if (!initializeWindow() || !initializeGlad()) {
		system("pause");
		return -1;
	}

	initializeLineObjects();
	initializeChargeObjects();

	while (!glfwWindowShouldClose(window)) {
		processWindowInput();

		if (viewMatrixChanged) {
			Camera::updateViewMatrix();
		}

		glClear(GL_COLOR_BUFFER_BIT);
		renderScreen();

		viewMatrixChanged = false;
		projectionMatrixChanged = false;

		glfwSwapBuffers(window);
		glfwPollEvents();
		limitFrameRate();
	}

	LineData::clear();
	ChargeData::clear();
	lineShader.clear();
	chargeShader.clear();
	glfwTerminate();
	return 0;
}



bool initializeWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Field Lines", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		Camera::updateProjectionMatrix(width, height);
		projectionMatrixChanged = true;
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			windowKeyPressed(key);
		}
	});

	Camera::updateProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

bool initializeGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	return true;
}

void initializeLineObjects() {
	lineShader.initialize("line", false);
	lineShader.activate();

	unsigned int stride = 3 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void initializeChargeObjects() {
	chargeShader.initialize("charge", true);
	chargeShader.activate();

	chargeShader.setFloat("radius", CHARGE_RADIUS);
	chargeShader.setFloat("coloredRadius", CHARGE_COLORED_RADIUS);

	unsigned int stride = 3 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}



void windowKeyPressed(int key) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (key == GLFW_KEY_SPACE) {
		for (unsigned int i = 0; i < ChargeData::count; i++) {
			ChargeData::charges[i].position = Vec2::random();
		}
		LineData::calculateVertices();
		ChargeData::calculateVertices();
	}
}

void processWindowInput() {
	if (isKeyPressed(GLFW_KEY_KP_ADD)) {
		Camera::zoom *= 1 + CAMERA_ZOOM * deltaTime;
		viewMatrixChanged = true;
	} else if (isKeyPressed(GLFW_KEY_KP_SUBTRACT)) {
		Camera::zoom *= 1 - CAMERA_ZOOM * deltaTime;
		viewMatrixChanged = true;
	}

	if (isKeyPressed(GLFW_KEY_LEFT) || isKeyPressed(GLFW_KEY_RIGHT)) {
		float change = CAMERA_MOVE * deltaTime / Camera::zoom;
		if (isKeyPressed(GLFW_KEY_RIGHT)) {
			change *= -1;
		}
		if (Camera::aspectRatio < 1) {
			change /= Camera::aspectRatio;
		}
		Camera::position.x += change;
		viewMatrixChanged = true;
	}

	if (isKeyPressed(GLFW_KEY_UP) || isKeyPressed(GLFW_KEY_DOWN)) {
		float change = CAMERA_MOVE * deltaTime / Camera::zoom;
		if (isKeyPressed(GLFW_KEY_UP)) {
			change *= -1;
		}
		if (Camera::aspectRatio > 1) {
			change *= Camera::aspectRatio;
		}
		Camera::position.y += change;
		viewMatrixChanged = true;
	}
}

bool isKeyPressed(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}



void renderScreen() {
	activateShader(lineShader);
	for (unsigned int line = 0; line < LineData::count; line++) {
		unsigned int segments = LineData::segmentCount[line];
		glBufferData(GL_ARRAY_BUFFER, segments * 3 * sizeof(float), LineData::vertices[line], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 0, segments);
	}

	activateShader(chargeShader);
	glBufferData(GL_ARRAY_BUFFER, ChargeData::count * 3 * sizeof(float), ChargeData::vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, ChargeData::count);
}

void activateShader(Shader shader) {
	shader.activate();

	if (viewMatrixChanged) {
		shader.setMat4("view", Camera::viewMatrix);
	}

	if (projectionMatrixChanged) {
		shader.setMat4("projection", Camera::projectionMatrix);

		if (shader == chargeShader) {
			shader.setMat4("inverseProjection", inverse(Camera::projectionMatrix));
			shader.setVec2("screen", (float)Camera::width, (float)Camera::height);
		}
	}
}

void limitFrameRate() {
	double newTime = glfwGetTime();
	deltaTime = (float)(newTime - lastTimeStamp);
	lastTimeStamp = newTime;

	if (deltaTime < MIN_DELTA_TIME) {
		std::this_thread::sleep_for(std::chrono::milliseconds((int)((MIN_DELTA_TIME - deltaTime) * 1000)));
		lastTimeStamp += MIN_DELTA_TIME - deltaTime;
		deltaTime = MIN_DELTA_TIME;
	}
}
