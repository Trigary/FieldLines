#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class shader {
public:
	GLenum buffer;
	unsigned int id = 0;
	unsigned int vao = 0;
	unsigned int vbo = 0;

	explicit shader(GLenum buffer);

	void initialize(const std::string& name, bool use_geometry_shader);
	void activate_shader() const;
	void activate() const;
	void clear() const;
	bool operator==(const shader& shader) const;

	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set_vec2(const std::string& name, glm::vec2& value) const;
	void set_vec2(const std::string& name, float x, float y) const;
	void set_vec3(const std::string& name, const glm::vec3& value) const;
	void set_vec3(const std::string& name, float x, float y, float z) const;
	void set_vec4(const std::string& name, const glm::vec4& value) const;
	void set_vec4(const std::string& name, float x, float y, float z, float w) const;
	void set_mat2(const std::string& name, const glm::mat2& mat) const;
	void set_mat3(const std::string& name, const glm::mat3& mat) const;
	void set_mat4(const std::string& name, const glm::mat4& mat) const;



private:
	static unsigned int create_shader(const std::string& name, const std::string& type, GLenum e_type);
	static std::string read_file_contents(const std::string& name, const std::string& type);
	static void check_compile_errors(unsigned int shader, const std::string& type);
};
