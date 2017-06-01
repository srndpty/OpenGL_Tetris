// Mino.h
// ブロック1個分を表す

#pragma once

#include "Sprite.h"

// テトリミノ1個分のオブジェクト
class Mino : public Sprite<4>
{
public:
	static const int VERTS_COUNT = 4;

	Vec2i mPosition{}; // 位置
	Vec2i mOffset{}; // テトリミノにおける、中心のブロックの位置から見たoffset

private:
	int mDropCount = 0;


public:
	/// special
	Mino();
	Mino(Vec2f aPos);
	Mino(int aX, int aY);
	~Mino();

	/// normal
	void Move(const Vec2i& amount);
	void SetPos(const Vec2i& pos);

private:
	void InitInfo(Vec2f aSize, Vec2f aPos);
	void ApplyPos();
};


