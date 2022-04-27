#pragma once

struct Move {
	int startX;
	int startY;
	int targetX;
	int targetY;

	bool capture = false;

	char promotion = '\0';
};