#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <unordered_map>



using namespace glm;
static const char* generic_shader_vert = R"(
#version 150 core
in vec3 v_position;
in vec2 v_uv;
in vec4 v_colour;
out vec2 f_uv;
out vec4 f_colour;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	f_uv = v_uv;
	f_colour = v_colour;
	gl_Position = (u_projection * u_view * u_model) * vec4(v_position.xyz, 1.0);
})";
static const char* generic_shader_frag = R"(
#version 150 core
uniform sampler2D u_texture;
in vec2 f_uv;
in vec4 f_colour;

out vec4 o_colour;
void main()
{
	o_colour = texture(u_texture, f_uv) * f_colour;
	if (o_colour.a == 0.0)
		discard;
})";

class Shader {
public:
	GLuint program = 0;
	GLuint vertex = 0;
	GLuint fragment = 0;

	const char* vert;
	const char* frag;

	std::unordered_map<std::string, GLint> uniform_map;

	Shader() {

	}

	~Shader() {
		glDeleteProgram(this->program);
		glDeleteShader(this->vertex);
		glDeleteShader(this->fragment);
	}

	void GL_Use()
	{
		glUseProgram(program);
	}

	GLint GetUniformLocation(std::string loc)
	{
		glUseProgram(program);
		//Use previously defined location
		auto key_loc = uniform_map.find(loc);
		if (key_loc != uniform_map.end())
			return key_loc->second;

		//Find and remember location
		GLint uniform_loc = glGetUniformLocation(program, loc.c_str());
		if (uniform_loc == -1)
		{
			std::cout << "Failed to get uniform location of " + loc << std::endl;
			return -1;
		}
		uniform_map[loc] = uniform_loc;
		return uniform_loc;
	}

#define SETUNIFORM_GET_LOC_ID() \
			GLint loc_id = GetUniformLocation(loc); \
			if (loc_id == -1) \
				return true;

	bool SetUniform(std::string loc, int i0)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform1i(loc_id, i0);
		return false;
	}

	bool SetUniform(std::string loc, int i0, int i1)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform2i(loc_id, i0, i1);
		return false;
	}

	bool SetUniform(std::string loc, int i0, int i1, int i2)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform3i(loc_id, i0, i1, i2);
		return false;
	}

	bool SetUniform(std::string loc, int i0, int i1, int i2, int i3)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform4i(loc_id, i0, i1, i2, i3);
		return false;
	}

	bool SetUniform(std::string loc, float f0)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform1f(loc_id, f0);
		return false;
	}

	bool SetUniform(std::string loc, float f0, float f1)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform2f(loc_id, f0, f1);
		return false;
	}

	bool SetUniform(std::string loc, float f0, float f1, float f2)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform3f(loc_id, f0, f1, f2);
		return false;
	}

	bool SetUniform(std::string loc, float f0, float f1, float f2, float f3)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniform4f(loc_id, f0, f1, f2, f3);
		return false;
	}

	bool SetUniform(std::string loc, int count, float* ptr)
	{
		SETUNIFORM_GET_LOC_ID()
			glUniformMatrix4fv(loc_id, count, GL_FALSE, ptr);
		return false;
	}

	void GL_CompileShader(const char* src_vert, const char* src_frag)
	{
		glDeleteProgram(this->program);
		glDeleteShader(this->vertex);
		glDeleteShader(this->fragment);
		// defaults
		if (src_vert == NULL)
			src_vert = generic_shader_vert;
		if (src_frag == NULL)
			src_frag = generic_shader_frag;

		vert = src_vert;
		frag = src_frag;

		//Create shader
		GLint shader_status;
		program = glCreateProgram();

		//Compile vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &src_vert, NULL);
		glCompileShader(this->vertex);

		glGetShaderiv(this->vertex, GL_COMPILE_STATUS, &shader_status);
		if (shader_status != GL_TRUE)
		{
			char buffer[0x200];
			glGetShaderInfoLog(this->vertex, sizeof(buffer), NULL, buffer);
			std::cout << buffer << std::endl;
		}

		//Compile fragment shader
		this->fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(this->fragment, 1, &src_frag, NULL);
		glCompileShader(this->fragment);

		glGetShaderiv(this->fragment, GL_COMPILE_STATUS, &shader_status);
		if (shader_status != GL_TRUE)
		{
			char buffer[0x200];
			glGetShaderInfoLog(this->fragment, sizeof(buffer), NULL, buffer);
			std::cout << buffer << std::endl;
		}

		//Attach and link
		glAttachShader(this->program, this->vertex);
		glAttachShader(this->program, this->fragment);

		glBindAttribLocation(this->program, 0, "v_position");
		glBindAttribLocation(this->program, 1, "v_uv");
		glBindAttribLocation(this->program, 2, "v_colour");

		glLinkProgram(this->program);

		glGetProgramiv(this->program, GL_LINK_STATUS, &shader_status);
		if (shader_status != GL_TRUE)
		{
			char buffer[0x200];
			glGetProgramInfoLog(this->program, sizeof(buffer), NULL, buffer);
			std::cout << buffer << std::endl;
		}

		std::cout << "Created shader " << program << std::endl;
	}

	void SetProject(Shader* shad, glm::mat4 proj)
	{
		shad->GL_Use();
		glUniformMatrix4fv(glGetUniformLocation(shad->program, "u_projection"), 1, GL_FALSE, &proj[0][0]);
	}
};

class GL
{
public:
	static Shader* genShader;
	static mat4 projection;
};


class Texture
{
public:
	//GL object
	GLuint id;

	//Constructors and destructor

	static Texture* createWithImage(std::string filePath);

	int width;
	int height;
	bool fromSTBI = false;
	unsigned char* pixels;

	Texture(unsigned char* data, const unsigned int width, const unsigned int height);
	~Texture();

	//Texture interface
	bool SetData(unsigned char* data, const unsigned int width, const unsigned int height);
	bool Bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
		return false;
	}
	bool Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		return false;
	}
};

struct GL_Vertex
{
	float x = 0, y = 0, z = 0;
	float u = 0, v = 0;
	float r = 255, g = 255, b = 255, a = 1;
};

struct Rect
{
	float x = 0, y = 0, z = 0;
	float w = 0, h = 0;
	float r = 255, g = 255, b = 255;
	float a = 1;
};

class Rendering
{
public:
	enum DepthCompare
	{
		DepthCompare_Never,
		DepthCompare_Less,
		DepthCompare_Equal,
		DepthCompare_LessEqual,
		DepthCompare_Greater,
		DepthCompare_NotEqual,
		DepthCompare_GreaterEqual,
		DepthCompare_Always,
	};

	static std::vector<GL_Vertex> batch_buffer;
	static GLuint batch_vao;
	static GLuint batch_vbo;
	static Shader* batch_shader;
	static Texture* batch_texture;
	static Texture* white;

	static void push(GL_Vertex tl, GL_Vertex bl, GL_Vertex tr, GL_Vertex br);

	static void Render_GLInit(Shader* shad);

	static void drawBatch();

	static void setBlend();
	static void setBlendSep();
	static void setPremBlend();

	static void SetClipRect(Rect* clipRect);

	static void PushQuad(Rect* dstRect, Rect* srcRect, Texture* tex, Shader* shad, float deg);
	static void PushQuad(Rect* dstRect, Rect* srcRect, Texture* tex, Shader* shad);
};