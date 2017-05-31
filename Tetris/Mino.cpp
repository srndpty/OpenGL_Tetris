#include "Game.h"
#include "Mino.h"


Mino::Mino(Vec2f aSize, Vec2f aPos)
{
	InitInfo(aSize, aPos);
}

Mino::Mino(Vec2f aSize, int aX, int aY)
{
	mPosition = { aX, aY };
	Vec2f pos { Game::FIELD_BOT_LEFT.x + Game::BLOCK_SIZE.x / 2 * aX,
		Game::FIELD_BOT_LEFT.y + Game::BLOCK_SIZE.y / 2 * aY };
	InitInfo(aSize, pos);
}

Mino::~Mino()
{
}

void Mino::Drop()
{
	++mDropCount;

	if (mPosition.y > 0 && (mDropCount & 63) == 0)
	{
		--mPosition.y;
		ApplyPos();
	}
}

void Mino::InitInfo(Vec2f aSize, Vec2f aPos)
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

void Mino::ApplyPos()
{
	pos = { Game::FIELD_BOT_LEFT.x + Game::BLOCK_SIZE.x / 2 * mPosition.x,
		Game::FIELD_BOT_LEFT.y + Game::BLOCK_SIZE.y / 2 * mPosition.y };

	for (size_t i = 0; i < VERTS_COUNT; i++)
	{
		geom[i].x = pos.x + vertex[i].x;
		geom[i].y = pos.y + vertex[i].y;
	}
}
