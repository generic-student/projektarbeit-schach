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
	std::array<std::array<char, 8>, 8> m_position;
	std::array<std::array<int, 8>, 8> m_moveCount;
	

	//Methoden
public:
	void setFEN(std::string p_FEN);
	void setActivePlayer(int p_id);
	std::array<std::array<int, 8>, 8>& getMoveCount();
	std::array<std::array<char, 8>, 8>& getPosition();
	std::string getFEN();
	char getType(int p_x, int p_y);
	int getMoveCountPos(int p_x, int p_y);
	int getActivePlayer();
	int getMoveNumber();
	float getPositionEvaluation();
	bool isViableMove(std::string p_Move);		// move is in form "old Position"-"new Position"  (e3-e4)
	std::list<Move> getValidMoves(int _startX, int _startY);

private:

	void FENToArray(std::string); //TODO
	std::string arrayToFEN();     //TODO
	Move parseMove(std::string p_Move);

};


