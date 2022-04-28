#pragma once
#include <string>
#include <array>
#include "move.hpp"
#include <list>

class Chessposition
{
	//Variablen
public :
	static const std::string STARTPOS_FEN;

private:
	unsigned int m_activePlayer;
	unsigned int m_moveNumber;					//Based on White Player (negative Numbers are an advantage for Black, positive advantage for white)
    std::string m_FENString;
	std::array<std::array<char, 8>, 8> m_position;
	std::array<std::array<int, 8>, 8> m_moveCount;
	

	//Methoden
public:
	void setFEN(std::string p_FEN);
	void setActivePlayer(int p_id);
	const std::array<std::array<int, 8>, 8>& getMoveCount() const;
	const std::array<std::array<char, 8>, 8>& getPosition() const;
	const std::string& getFEN() const;
	char getType(int p_x, int p_y) const;
	int getMoveCountPos(int p_x, int p_y) const;
	int getActivePlayer() const;
	int getMoveNumber() const;
	bool isViableMove(const Move& move) const;		// move is in form "old Position"-"new Position"  (e3-e4)
	std::list<Move> getValidMoves(int _startX, int _startY) const;
	bool applyMove(const Move& move, bool validate);

private:
	Move parseMove(std::string p_Move);

};

