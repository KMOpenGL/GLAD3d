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
void Rendering::SetClipRect(Rect* clipRect)
{
	drawBatch();
	if (clipRect)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(clipRect->x, 720 - clipRect->h - clipRect->y, clipRect->w, clipRect->h);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
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

// rotate the texture on 90 degree angles
void Rendering::PushQuad(Rect* dstRect, Rect* srcRect, Texture* tex, Shader* shad, float deg)
{

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

	GL_Vertex tl;
	tl.x = dstRect->x;
	tl.y = dstRect->y;
	tl.u = srcRect->x;
	tl.v = srcRect->y;
	tl.r = dstRect->r / 255;
	tl.g = dstRect->g / 255;
	tl.b = dstRect->b / 255;
	tl.a = dstRect->a;

	GL_Vertex bl;
	bl.x = dstRect->x;
	bl.y = dstRect->y + dstRect->h;
	bl.u = srcRect->x;
	bl.v = srcRect->y + srcRect->h;
	bl.r = dstRect->r / 255;
	bl.g = dstRect->g / 255;
	bl.b = dstRect->b / 255;
	bl.a = dstRect->a;

	GL_Vertex tr;
	tr.x = dstRect->x + dstRect->w;
	tr.y = dstRect->y;
	tr.u = srcRect->x + srcRect->w;
	tr.v = srcRect->y;
	tr.r = dstRect->r / 255;
	tr.g = dstRect->g / 255;
	tr.b = dstRect->b / 255;
	tr.a = dstRect->a;

	GL_Vertex br;
	br.x = dstRect->x + dstRect->w;
	br.y = dstRect->y + dstRect->h;
	br.u = srcRect->x + srcRect->w;
	br.v = srcRect->y + srcRect->h;
	br.r = dstRect->r / 255;
	br.g = dstRect->g / 255;
	br.b = dstRect->b / 255;
	br.a = dstRect->a;

	// rotate stuff

	if (deg != 0) // lil broken too
	{
		float s = sin(deg * (3.14159265 / 180));
		float c = cos(deg * (3.14159265 / 180));
		float cx = dstRect->x + dstRect->w * 0.5f;
		float cy = dstRect->y + dstRect->h * 0.5f;

		GL_Vertex* verts[] = { &tl, &bl, &tr, &br };
		for (GL_Vertex* vert : verts)
		{
			float tx = vert->x - cx;
			float ty = vert->y - cy;
			float rx = tx * c - ty * s;
			float ry = tx * s + ty * c;
			vert->x = rx + cx;
			vert->y = ry + cy;
		}
	}


	batch_buffer.push_back(tl);
	batch_buffer.push_back(bl);
	batch_buffer.push_back(tr);
	batch_buffer.push_back(tr);
	batch_buffer.push_back(bl);
	batch_buffer.push_back(br);
	
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

	push(face1_tl, face1_bl, face1_tr, face1_tr);

	// left side

	GL_Vertex face2_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_bl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_tr = { pos.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face2_br = { pos.x, pos.y + stuff.y, pos.z, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face2_tl, face2_bl, face2_tr, face2_tr);

	// right side

	GL_Vertex face3_tl = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_bl = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_tr = { pos.x + stuff.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face3_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face3_tl, face3_bl, face3_tr, face3_tr);

	// back side

	GL_Vertex face4_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_bl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,1, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_tr = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face4_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 1,1, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face4_tl, face4_bl, face4_tr, face4_tr);

	// top side

	GL_Vertex face5_tl = { pos.x, pos.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_bl = { pos.x, pos.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_tr = { pos.x + stuff.x, pos.y, pos.z, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face5_br = { pos.x + stuff.x, pos.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face5_tl, face5_bl, face5_tr, face5_tr);

	// bottom side

	GL_Vertex face6_tl = { pos.x, pos.y + stuff.y, pos.z - stuff.x, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_bl = { pos.x, pos.y + stuff.y, pos.z, 0,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_tr = { pos.x + stuff.x, pos.y + stuff.y, pos.z, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };
	GL_Vertex face6_br = { pos.x + stuff.x, pos.y + stuff.y, pos.z - stuff.x, 1,0, color.r / 255, color.g / 255, color.b / 255, color.a };

	push(face6_tl, face6_bl, face6_tr, face6_tr);
}