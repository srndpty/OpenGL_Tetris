#pragma once

#include "Vec2.h"
#include "linmath.h"

// base class for sprite object
template<int I>
class Sprite
{
public:
	Sprite() = default;
	virtual ~Sprite() = default;

	mat4x4 m, p, mvp;

	void Draw(int texId)
	{
		mat4x4_identity(m);
		mat4x4_translate_in_place(m, pos.x, pos.y, 0);
		mat4x4_ortho(p, -ASPECT_RATIO, ASPECT_RATIO, -1.f, 1.f, 1.f, -1.f);
		mat4x4_mul(mvp, p, m);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(shader.mMvpLocation, 1, false, (const GLfloat*)mvp);
		// attributeëÆê´Çìoò^
		glVertexAttribPointer(shader.mPositionLocation, 2, GL_FLOAT, false, 0, geom);
		glVertexAttribPointer(shader.mUvLocation, 2, GL_FLOAT, false, 0, uv);

		// ÉÇÉfÉãÇÃï`âÊ
		glBindTexture(GL_TEXTURE_2D, texId);
		glDrawArrays(GL_TRIANGLE_FAN, 0, I);
	}

public:
	Vec2 size{};
	Vec2 pos{}; // ç¿ïW
	Vec2 vertex[I]{}; // offset
	Vec2 geom[I]{}; // é¿ç€ÇÃíl
	Vec2 uv[I]{}; // uv
};

