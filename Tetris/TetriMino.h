// TetriMino.h
// 通常4つあわさったブロック（テトリミノ）を管理する

#pragma once

#include <cassert>
#include <memory>

#include "Mino.h"

struct MinoType;

// テトリミノを表現するクラス
class TetriMino
{
public:
	static const int MINO_TYPE_MAX = 7;
	static const int MINO_MAX = 4;
	static MinoType minoTypes[MINO_TYPE_MAX]; // テトリミノのパターン一覧

public:
	Vec2i mPosition; // 基準の位置
	std::unique_ptr<Mino> mMinos[MINO_MAX]; // 所持する4つのブロック
	int mType; // テトリミノのタイプ
	int mNextType; // 次に降ってくるミノのタイプ

public:
	/// special
	TetriMino();
	TetriMino(Vec2i aPos);
	~TetriMino();

	/// normal
	void SetType(int type); // テトリミノのタイプを設定
	void SetPos(const Vec2i& pos); // 基準位置の設定
	void SetForcePosition(const Vec2f& aPos); // 強制的に座標情報を上書き（次のミノ用）
	void Move(const Vec2i& amount); // 移動
	void Draw(int texId); // 描画
	void Rotate(); // 回転

	// get/set
	int GetNextType() const { return mNextType; }
};

// テトリミノのタイプを表現するクラス
struct MinoType
{
	Vec2i offset[TetriMino::MINO_MAX];
	Vec4f color; // 色（RGBA）
};


