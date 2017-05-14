#include "Mino.h"

float Mino::BLOCK_SIDE_LENGTH = 0.09f;
Vec2f Mino::BLOCK_SIZE = { BLOCK_SIDE_LENGTH, BLOCK_SIDE_LENGTH };
Vec2f Mino::FIELD_BOT_LEFT = { -0.5f, -0.42f };

Mino::Mino(Vec2f aSize, Vec2f aPos)
{
	vertex[0] = geom[0] = { -aSize.x / 2, +aSize.y / 2 };
	vertex[1] = geom[1] = { +aSize.x / 2, +aSize.y / 2 };
	vertex[2] = geom[2] = { +aSize.x / 2, -aSize.y / 2 };
	vertex[3] = geom[3] = { -aSize.x / 2, -aSize.y / 2 };
	uv[0] = { 0, 1 };
	uv[1] = { 1, 1 };
	uv[2] = { 1, 0 };
	uv[3] = { 0, 0 };
	pos = aPos;
	size = aSize * 0.5f;
}

Mino::~Mino()
{
}
