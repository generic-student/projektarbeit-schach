#pragma once

struct Move {
	int startX;
	int startY;
	int targetX;
	int targetY;

	bool capture;

	char promotion;
};