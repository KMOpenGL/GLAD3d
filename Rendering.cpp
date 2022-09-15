#include "Gl.h"
std::vector<GL_Vertex> Rendering::batch_buffer;
GLuint Rendering::batch_vao = -1;
GLuint Rendering::batch_vbo = -1;
Shader* Rendering::batch_shader = NULL;
Texture* Rendering::batch_texture = NULL;
Texture* Rendering::white = NULL;

void Rendering::push(GL_Vertex tl, GL_Vertex bl, GL_Vertex tr, GL_Vertex br)
{
	batch_buffer.push_back(tl);
	batch_buffer.push_back(bl);
	batch_buffer.push_back(tr);
	batch_buffer.push_back(tr);
	batch_buffer.push_back(bl);
	batch_buffer.push_back(br);
}

void Rendering::Render_GLInit(Shader* shad) {
	
	shad->GL_Use();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	unsigned char c[] = { 255, 255, 255, 255 };

	white = new Texture(c, 1, 1);

	glGenVertexArrays(1, &batch_vao);
	glBindVertexArray(batch_vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &batch_vbo);
	
}

void Rendering::drawBatch()
{
	
	if (batch_buffer.size() != 0)
	{
		glBindVertexArray(batch_vao);
		glBindBuffer(GL_ARRAY_BUFFER, batch_vbo);

		batch_shader->GL_Use();
		batch_texture->Bind();

		//Set attribute pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex), (GLvoid*)offsetof(GL_Vertex, x));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex), (GLvoid*)offsetof(GL_Vertex, u));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GL_Vertex), (GLvoid*)offsetof(GL_Vertex, r));

		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_Vertex) * batch_buffer.size(), batch_buffer.data(), GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLES, 0, batch_buffer.size());
		batch_buffer.clear(); // clear it out
	}
	batch_texture = NULL;
	batch_shader = NULL;
	
}

void Rendering::setBlend() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Rendering::setPremBlend() {
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Rendering::setBlendSep() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}


void Rendering::PushQuad(Rect* dstRect, Rect* srcRect, Texture* tex, Shader* shad) {

	
	if (tex == nullptr)
		tex = white;
	if (shad == nullptr)
		shad = GL::genShader;

	if (batch_texture != tex || batch_shader != shad)
	{
		drawBatch();

		batch_texture = tex;
		batch_shader = shad;
	}

	vec3 pos = { dstRect->x, dstRect->y, dstRect->z };
	vec4 color = { dstRect->r,dstRect->g,dstRect->b, dstRect->a };
	vec2 stuff = { dstRect->w, dstRect->h };

	// front side

	GL_Vertex face1_tl = { pos.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a};
	GL_Vertex face1_bl = { pos.x, pos.y + stuff.y, pos.z, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face1_tr = { pos.x + stuff.x, pos.y, pos.z, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face1_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z, 1,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face1_tl, face1_bl, face1_tr, face1_br);

	// left side

	GL_Vertex face2_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_bl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_tr = { pos.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_br = { pos.x, pos.y + stuff.y, pos.z, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face2_tl, face2_bl, face2_tr, face2_br);

	// right side

	GL_Vertex face3_tl = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_bl = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_tr = { pos.x + stuff.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face3_tl, face3_bl, face3_tr, face3_br);

	// back side

	GL_Vertex face4_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_bl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_tr = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 1,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face4_tl, face4_bl, face4_tr, face4_br);

	// top side

	GL_Vertex face5_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_bl = { pos.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_tr = { pos.x + stuff.x, pos.y, pos.z, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_br = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face5_tl, face5_bl, face5_tr, face5_br);

	// bottom side

	GL_Vertex face6_tl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_bl = { pos.x, pos.y + stuff.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_tr = { pos.x + stuff.x, pos.y + stuff.y, pos.z, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face6_tl, face6_bl, face6_tr, face6_br);
}