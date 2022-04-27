#pragma once
#include <string>
#include <array>
#include "move.hpp"
#include <list>

class Chessposition
{
	//Variablen
public :


private:
	unsigned int m_activePlayer;
	unsigned int m_moveNumber;
	float m_positionEvaluation;						//Based on White Player (negative Numbers are an advantage for Black, positive advantage for white)
    std::string m_FENString;
	std::array<std::array<char, 8>, 8> m_Position;



	//Methoden
public:
	std::string getFEN();
	char getType(int x, int y);
	int getActivePlayer();
	int getMoveNumber();
	float getPositionEvaluation();
	bool isViableMove(std::string p_Move);		// move is in form "old Position"-"new Position"  (e3-e4)
	std::list<Move> getValidMoves(int _startX, int _startY);
	bool checkValidMove(Move move);

private:

	void FENToArray(std::string);
	std::string arrayToFEN();
	Move parseMove(std::string p_Move);

};


