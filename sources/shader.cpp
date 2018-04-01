#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

shader::shader(GLenum buffer) {
	this->buffer = buffer;
}



void shader::initialize(const std::string& name, bool use_geometry_shader) {
	unsigned int vertex = create_shader(name, "vertex", GL_VERTEX_SHADER);
	unsigned int geometry = 0;
	if (use_geometry_shader) {
		geometry = create_shader(name, "geometry", GL_GEOMETRY_SHADER);
	}
	unsigned int fragment = create_shader(name, "fragment", GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	if (use_geometry_shader) {
		glAttachShader(id, geometry);
	}
	glAttachShader(id, fragment);

	glLinkProgram(id);
	check_compile_errors(id, "program");

	glDeleteShader(vertex);
	if (use_geometry_shader) {
		glDeleteShader(geometry);
	}
	glDeleteShader(fragment);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

void shader::activate_shader() const {
	glUseProgram(id);
}

void shader::activate() const {
	glUseProgram(id);
	glBindVertexArray(vao);
	glBindBuffer(buffer, vbo);
}

void shader::clear() const {
	glDeleteProgram(id);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

bool shader::operator==(const shader& shader) const {
	return shader.id == this->id;
}



void shader::set_bool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void shader::set_int(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_float(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_vec2(const std::string& name, glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void shader::set_vec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void shader::set_vec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void shader::set_vec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void shader::set_mat2(const std::string& name, const glm::mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::set_mat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void shader::set_mat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}



unsigned int shader::create_shader(const std::string& name, const std::string& type, GLenum e_type) {
	std::string code = read_file_contents(name, type);
	const char* pointer = code.c_str();
	unsigned int id = glCreateShader(e_type);
	glShaderSource(id, 1, &pointer, nullptr);
	glCompileShader(id);
	check_compile_errors(id, type);
	return id;
}

std::string shader::read_file_contents(const std::string& name, const std::string& type) {
	try {
		std::ifstream file_stream;
		file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file_stream.open("shader." + name + "." + type);
		std::stringstream string_stream;
		string_stream << file_stream.rdbuf();
		file_stream.close();
		return string_stream.str();
	} catch (std::ifstream::failure&) {
		std::cout << "Error while reading shader file: " << name << std::endl;
		return nullptr;
	}
}

void shader::check_compile_errors(unsigned int shader, const std::string& type) {
	int success;
	if (type == "program") {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			char info_log[1024];
			glGetProgramInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "Program linking error:" << "\n" << info_log << std::endl;
		}
	} else {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char info_log[1024];
			glGetShaderInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "Shader compilation error for shader type: " << type << "\n" << info_log << std::endl;
		}
	}
}
