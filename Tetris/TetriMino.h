#pragma once

#include "Mino.h"

struct MinoType;

class TetriMino
{
public:
	static const int MINO_TYPE_MAX = 7;
	static const int MINO_MAX = 4;
	static MinoType minoTypes[MINO_TYPE_MAX];

public:
	Vec2i mPosition;
	Mino mMinos[MINO_MAX]{};
	int mType;

public:
	// special
	TetriMino();
	TetriMino(Vec2i aPos);
	~TetriMino();

	// normal
	void SetType(int type);
	void SetPos(const Vec2i& pos);
	void Move(const Vec2i& amount);
	void Draw(int texId);
};

struct MinoType
{
	Vec2i offset[TetriMino::MINO_MAX];
};


