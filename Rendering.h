#pragma once
#include "includes.h"
#include "Texture.h"
#include "Shader.h"

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