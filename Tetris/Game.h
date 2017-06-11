// Game.h
// ゲーム全体の制御

#pragma once

#include "Vec2.h"
#include "Mino.h"
#include "TetriMino.h"

// ゲームのロジック管理
class Game
{
public:
	static float BLOCK_SIDE_LENGTH;
	static Vec2f BLOCK_SIZE;
	static Vec2f FIELD_BOT_LEFT;

	static const int SENTINELS_COUNT = 2;
	static const int FIELD_WIDTH = 10 + SENTINELS_COUNT * 2;
	static const int FIELD_HEIGHT = 20 + SENTINELS_COUNT * 2; // ゲームオーバー判定用の上の部分を追加
	static const int DROP_INTERVAL = 20;
	static Vec2i FIELD_SIZE;
	static const int DELETE_LINE_POINTS = 300;
	static const int DROP_MINO_POINTS = 2;

	bool mExists[FIELD_HEIGHT][FIELD_WIDTH]; // 各マス目にブロックが存在するか否か
	bool mToBeDropped = false; // テトリミノの落下を試みるかどうか
	bool mToBeRotated = false; // テトリミノの回転を試みるかどうか

private:
	int mDropCount = 0; // 落下のためのフレームカウンタ

public:
	/// special
	Game();
	~Game();

	/// normal
	void Process(); // マイフレームの処理
	bool IsMovable(const Mino& mino, int horizontal, int vertical); // 移動可能判定
	bool IsMovable(const TetriMino& mino, int horizontal, int vertical); // 移動可能判定
	bool IsRotatable(const TetriMino& mino); // 回転可能判定
	void PlaceCurrent(const Mino& mino); // 現在のオブジェクトを今の位置で設置確定させる
	void PlaceCurrent(const TetriMino& mino); // 現在のオブジェクトを今の位置で設置確定させる
	int DropLines(); // そろっているラインを検出し、ブロックを全体的に卸す ret: 消した行数
	bool IsGameOver(); // ゲームオーバー判定

private:
	bool IsLineFilled(int y); // 指定の列が埋まっているかどうか
	void MoveLine(int from, int to); // ラインの移動

};

