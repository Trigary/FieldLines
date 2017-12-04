#pragma once

class Shader {
public:
	GLenum buffer;
	unsigned int shader = 0;
	unsigned int vao = 0;
	unsigned int vbo = 0;

	explicit Shader(GLenum buffer) {
		this->buffer = buffer;
	}



	void initialize(std::string name, bool useGeometryShader) {
		unsigned int vertex = createShader(name, "vertex", GL_VERTEX_SHADER);
		unsigned int geometry = 0;
		if (useGeometryShader) {
			geometry = createShader(name, "geometry", GL_GEOMETRY_SHADER);
		}
		unsigned int fragment = createShader(name, "fragment", GL_FRAGMENT_SHADER);

		shader = glCreateProgram();
		glAttachShader(shader, vertex);
		if (useGeometryShader) {
			glAttachShader(shader, geometry);
		}
		glAttachShader(shader, fragment);

		glLinkProgram(shader);
		checkCompileErrors(shader, "program");

		glDeleteShader(vertex);
		if (useGeometryShader) {
			glDeleteShader(geometry);
		}
		glDeleteShader(fragment);

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
	}

	void activateShader() const {
		glUseProgram(shader);
	}

	void activate() const {
		glUseProgram(shader);
		glBindVertexArray(vao);
		glBindBuffer(buffer, vbo);
	}

	void clear() const {
		glDeleteProgram(shader);
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	bool operator==(const Shader& shader) const {
		return shader.shader == this->shader;
	}



	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(shader, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(shader, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(shader, name.c_str()), value);
	}

	void setVec2(const std::string& name, const glm::vec2& value) const {
		glUniform2fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
	}

	void setVec2(const std::string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(shader, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(shader, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(shader, name.c_str()), 1, &value[0]);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(shader, name.c_str()), x, y, z, w);
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(shader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}



private:
	static unsigned int createShader(std::string name, std::string type, GLenum eType) {
		std::string code = readFileContents(name, type);
		const char* pointer = code.c_str();
		unsigned int id = glCreateShader(eType);
		glShaderSource(id, 1, &pointer, nullptr);
		glCompileShader(id);
		checkCompileErrors(id, type);
		return id;
	}

	static std::string readFileContents(std::string name, std::string type) {
		try {
			std::ifstream fileStream;
			fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fileStream.open("shader." + name + "." + type);
			std::stringstream stringStream;
			stringStream << fileStream.rdbuf();
			fileStream.close();
			return stringStream.str();
		} catch (std::ifstream::failure e) {
			std::cout << "Error while reading shader file: " << name << std::endl;
			return nullptr;
		}
	}

	static void checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		if (type == "program") {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				char infoLog[1024];
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "Program linking error:" << "\n" << infoLog << std::endl;
			}
		} else {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				char infoLog[1024];
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "Shader compilation error for shader type: " << type << "\n" << infoLog << std::endl;
			}
		}
	}
};
