#pragma once

#include "Sprite.h"

struct XY
{
	float x, y;
};

// テトリミノ1個分のオブジェクト
class Mino : public Sprite<4>
{
	// variables
public:
	// static
	static const int VERTS_COUNT = 4;

	Vec2i mPosition;

private:
	// non-static
	int mDropCount;


	// functions
public:
	// special
	Mino() = delete;
	Mino(Vec2f aSize, Vec2f aPos);
	Mino(Vec2f aSize, int aX, int aY);
	~Mino();

	// normal
	bool Drop();
	void Move(int x, int y);

private:
	void InitInfo(Vec2f aSize, Vec2f aPos);
	void ApplyPos();
};


