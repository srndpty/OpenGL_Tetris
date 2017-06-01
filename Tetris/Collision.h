// Collision.h
// Õ“Ë”»’è‚ğ‚Â‚©‚³‚Ç‚éƒtƒ@ƒCƒ‹

#pragma once

#include "Sprite.h"

template<int IA, int IB>
bool IsCollidingSqSq(Sprite<IA> a, Sprite<IB> b)
{
	if (b.pos.x + b.size.x / 2 > a.pos.x - a.size.x / 2)
	{
		if (b.pos.x - b.size.x / 2 < a.pos.x + a.size.x / 2)
		{
			if (b.pos.y + b.size.y / 2 > a.pos.y - a.size.y / 2)
			{
				if (b.pos.y - b.size.y / 2 < a.pos.y + a.size.y / 2)
				{
					return true;
				}
			}
		}
	}

	return false;
}

