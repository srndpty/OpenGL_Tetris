#pragma once

#include "Vec2.h"
#include "Mino.h"

class Game
{
public:
	static float BLOCK_SIDE_LENGTH;
	static Vec2f BLOCK_SIZE;
	static Vec2f FIELD_BOT_LEFT;

	static const int FIELD_WIDTH = 10 + 2;
	static const int FIELD_HEIGHT = 20 + 1;
	static const int DROP_INTERVAL = 20;
	static Vec2i FIELD_SIZE;

	bool mExists[FIELD_HEIGHT][FIELD_WIDTH];
	bool mToBeDropped = false;

private:
	int mDropCount = 0;

public:
	// special
	Game();
	~Game();

	// normal
	void Process();
	bool IsDroppable(const Mino& mino);
	bool IsMovable(const Mino& mino, int horizontal, int vertical);
	void PlaceCurrent(const Mino& mino);

};

