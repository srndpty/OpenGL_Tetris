
#include "TetriMino.h"


MinoType TetriMino::minoTypes[MINO_TYPE_MAX] =
{
	{ Vec2i{ 0, 0 },Vec2i{ 0,-1 },Vec2i{-1, 0 },Vec2i{+1, 0 }, }, // T
	{ Vec2i{ 0, 0 },Vec2i{ 0,-1 },Vec2i{ 0,-2 },Vec2i{ 0,+1 }, }, // I
	{ Vec2i{ 0, 0 },Vec2i{+1, 0 },Vec2i{ 0,-1 },Vec2i{+1,-1 }, }, // O
	{ Vec2i{ 0, 0 },Vec2i{ 0,+1 },Vec2i{ 0,-1 },Vec2i{+1,-1 }, }, // L
	{ Vec2i{ 0, 0 },Vec2i{ 0,+1 },Vec2i{ 0,-1 },Vec2i{-1,-1 }, }, // J
	{ Vec2i{ 0, 0 },Vec2i{+1, 0 },Vec2i{ 0,-1 },Vec2i{-1,-1 }, }, // S
	{ Vec2i{ 0, 0 },Vec2i{-1, 0 },Vec2i{ 0,-1 },Vec2i{+1,-1 }, }, // Z
};

TetriMino::TetriMino()
{
}


TetriMino::TetriMino(Vec2i aPos)
{
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i] = std::make_unique<Mino>(0, 0);
	}
	mPosition = aPos;
	SetType(0);
	SetPos(mPosition);
}

TetriMino::~TetriMino()
{
}

void TetriMino::SetType(int type)
{
	assert(type < MINO_TYPE_MAX);

	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->mOffset = minoTypes[type].offset[i];
	}
}

void TetriMino::SetPos(const Vec2i& pos)
{
	mPosition = pos;
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->SetPos(pos + mMinos[i]->mOffset);
	}
}

void TetriMino::Move(const Vec2i& amount)
{
	mPosition += amount;
	SetPos(mPosition);
}

void TetriMino::Draw(int texId)
{
	for (size_t i = 0; i < MINO_MAX; i++)
	{
		mMinos[i]->Draw(texId);
	}
}
