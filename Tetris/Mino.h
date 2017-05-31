#pragma once

#include "Sprite.h"

// テトリミノ1個分のオブジェクト
class Mino : public Sprite<4>
{
	// variables
public:
	// static
	static const int VERTS_COUNT = 4;

	Vec2i mPosition{};
	Vec2i mOffset{};

private:
	// non-static
	int mDropCount = 0;


	// functions
public:
	// special
	Mino();
	Mino(Vec2f aPos);
	Mino(int aX, int aY);
	~Mino();

	// normal
	void Move(const Vec2i& amount);
	void SetPos(const Vec2i& pos);

private:
	void InitInfo(Vec2f aSize, Vec2f aPos);
	void ApplyPos();
};


