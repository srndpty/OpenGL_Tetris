#pragma once

#include "Vec2.h"
#include "Mino.h"
#include "TetriMino.h"

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

	bool mExists[FIELD_HEIGHT][FIELD_WIDTH];
	bool mToBeDropped = false;
	bool mToBeRotated = false;

private:
	int mDropCount = 0;

public:
	// special
	Game();
	~Game();

	// normal
	void Process();
	bool IsMovable(const Mino& mino, int horizontal, int vertical);
	bool IsMovable(const TetriMino& mino, int horizontal, int vertical);
	bool IsRotatable(const TetriMino& mino);
	void PlaceCurrent(const Mino& mino);
	void PlaceCurrent(const TetriMino& mino);
	void DropLines();
	bool IsLineFilled(int y);
	void MoveLine(int from, int to);
	bool IsGameOver();
};

