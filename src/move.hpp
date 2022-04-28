#pragma once

struct Move {
	int startCol;
	int startRow;
	int targetCol;
	int targetRow;

	bool capture = false;
	int captureRow;
	int captureCol;


	char promotion = '\0';
};