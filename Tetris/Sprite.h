#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Vec2.h"
#include "linmath.h"
#include "Def.h"
#include "Shader.h"

extern Shader shader;

// base class for sprite object
template<int VERTS_COUNT>
class Sprite
{
public:
	Sprite() = default;
	virtual ~Sprite() = default;

	// •`‰æ
	void Draw(int texId)
	{
		// ”½‰f
		for (size_t i = 0; i < VERTS_COUNT; i++)
		{
			geom[i].x = pos.x + vertex[i].x;
			geom[i].y = pos.y + vertex[i].y;
		}

		mat4x4_identity(m);
		mat4x4_translate_in_place(m, pos.x, pos.y, 0);
		mat4x4_ortho(p, -ASPECT_RATIO, ASPECT_RATIO, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(shader.mMvpLocation, 1, false, (const GLfloat*)mvp);
		// attribute‘®«‚ð“o˜^
		glVertexAttribPointer(shader.mPositionLocation, 2, GL_FLOAT, false, 0, geom);
		glVertexAttribPointer(shader.mUvLocation, 2, GL_FLOAT, false, 0, uv);

		// ƒ‚ƒfƒ‹‚Ì•`‰æ
		glBindTexture(GL_TEXTURE_2D, texId);
		glDrawArrays(GL_TRIANGLE_FAN, 0, VERTS_COUNT);
	}

public:
	Vec2f size{};
	Vec2f pos{}; // À•W
	Vec2f vertex[VERTS_COUNT]{}; // offset
	Vec2f geom[VERTS_COUNT]{}; // ŽÀÛ‚Ì’l
	Vec2f uv[VERTS_COUNT]{}; // uv

private:
	mat4x4 m, p, mvp;
};

