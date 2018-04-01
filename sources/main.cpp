#include <ctime>
#include <thread>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.inl>
#include "constants.h"
#include "camera.hpp"
#include "charge_data.hpp"
#include "line_data.hpp"
#include "io.hpp"
#include "shader.hpp"

bool initialize_window();
bool initialize_glad();
void initialize_line_objects();
void initialize_charge_objects();
void window_key_pressed(int key);
void process_window_input();
bool is_key_pressed(int key);
void render_screen();
void activate_shader(shader shader);
void limit_frame_rate();

GLFWwindow* window;
shader line_shader(GL_ARRAY_BUFFER);
shader charge_shader(GL_ARRAY_BUFFER);
double last_time_stamp = 0;
float delta_time = 0.0f;
bool view_matrix_changed = true;
bool projection_matrix_changed = true;



int main() {
	srand((unsigned int)time(nullptr));
	io::print_info();
	io::initialize_charges();

	if (!initialize_window() || !initialize_glad()) {
		system("pause");
		return -1;
	}

	initialize_line_objects();
	initialize_charge_objects();

	while (!glfwWindowShouldClose(window)) {
		process_window_input();

		if (view_matrix_changed) {
			camera::update_view_matrix();
		}

		glClear(GL_COLOR_BUFFER_BIT);
		render_screen();

		view_matrix_changed = false;
		projection_matrix_changed = false;

		glfwSwapBuffers(window);
		glfwPollEvents();
		limit_frame_rate();
	}

	line_data::clear();
	charge_data::clear();
	line_shader.clear();
	charge_shader.clear();
	glfwTerminate();
	return 0;
}


bool initialize_window() {
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
		camera::update_projection_matrix(width, height);
		projection_matrix_changed = true;
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			window_key_pressed(key);
		}
	});

	camera::update_projection_matrix(SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

bool initialize_glad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	return true;
}

void initialize_line_objects() {
	line_shader.initialize("line", false);
	line_shader.activate();

	unsigned int stride = 3 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void initialize_charge_objects() {
	charge_shader.initialize("charge", true);
	charge_shader.activate();

	charge_shader.set_float("radius", CHARGE_RADIUS);
	charge_shader.set_float("colored_radius", CHARGE_COLORED_RADIUS);

	unsigned int stride = 3 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)(nullptr));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
}


void window_key_pressed(int key) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (key == GLFW_KEY_SPACE) {
		for (unsigned int i = 0; i < charge_data::count; i++) {
			charge_data::charges[i].position = vec2::random();
		}
		line_data::calculate_vertices();
		charge_data::calculate_vertices();
	}
}

void process_window_input() {
	if (is_key_pressed(GLFW_KEY_KP_ADD)) {
		camera::zoom *= 1 + CAMERA_ZOOM * delta_time;
		view_matrix_changed = true;
	} else if (is_key_pressed(GLFW_KEY_KP_SUBTRACT)) {
		camera::zoom *= 1 - CAMERA_ZOOM * delta_time;
		view_matrix_changed = true;
	}

	if (is_key_pressed(GLFW_KEY_LEFT) || is_key_pressed(GLFW_KEY_RIGHT)) {
		float change = CAMERA_MOVE * delta_time / camera::zoom;
		if (is_key_pressed(GLFW_KEY_RIGHT)) {
			change *= -1;
		}
		if (camera::aspect_ratio < 1) {
			change /= camera::aspect_ratio;
		}
		camera::position.x += change;
		view_matrix_changed = true;
	}

	if (is_key_pressed(GLFW_KEY_UP) || is_key_pressed(GLFW_KEY_DOWN)) {
		float change = CAMERA_MOVE * delta_time / camera::zoom;
		if (is_key_pressed(GLFW_KEY_UP)) {
			change *= -1;
		}
		if (camera::aspect_ratio > 1) {
			change *= camera::aspect_ratio;
		}
		camera::position.y += change;
		view_matrix_changed = true;
	}
}

bool is_key_pressed(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}


void render_screen() {
	activate_shader(line_shader);
	for (unsigned int line = 0; line < line_data::count; line++) {
		unsigned int segments = line_data::segment_count[line];
		glBufferData(GL_ARRAY_BUFFER, segments * 3 * sizeof(float), line_data::vertices[line], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_LINE_STRIP, 0, segments);
	}

	activate_shader(charge_shader);
	glBufferData(GL_ARRAY_BUFFER, charge_data::count * 3 * sizeof(float), charge_data::vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, charge_data::count);
}

void activate_shader(shader shader) {
	shader.activate();

	if (view_matrix_changed) {
		shader.set_mat4("view", camera::view_matrix);
	}

	if (projection_matrix_changed) {
		shader.set_mat4("projection", camera::projection_matrix);

		if (shader == charge_shader) {
			shader.set_mat4("inverse_projection", inverse(camera::projection_matrix));
			shader.set_vec2("screen", (float)camera::width, (float)camera::height);
		}
	}
}

void limit_frame_rate() {
	double new_time = glfwGetTime();
	delta_time = (float)(new_time - last_time_stamp);
	last_time_stamp = new_time;

	if (delta_time < MIN_DELTA_TIME) {
		std::this_thread::sleep_for(std::chrono::milliseconds((int)((MIN_DELTA_TIME - delta_time) * 1000)));
		last_time_stamp += MIN_DELTA_TIME - delta_time;
		delta_time = MIN_DELTA_TIME;
	}
}
