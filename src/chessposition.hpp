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

	explicit Chessposition();
	explicit Chessposition(const std::string& fen);
	explicit Chessposition(const std::array<std::array<char, 8>, 8>& pos);

private:
	unsigned int m_activePlayer = 0;
	unsigned int m_moveNumber = 0;					//Based on White Player (negative Numbers are an advantage for Black, positive advantage for white)
	std::array<std::array<char, 8>, 8> m_position;
	std::array<std::array<int, 8>, 8> m_moveCount;
	

	//Methoden
public:
	void setFEN(std::string p_FEN);
	std::string getFEN() const;

	void setActivePlayer(int p_id);
	int getActivePlayer() const;

	const std::array<std::array<int, 8>, 8>& getMoveCount() const;
	const std::array<std::array<char, 8>, 8>& getPosition() const;
	char getType(int p_x, int p_y) const;
	
	int getMoveCountPos(int p_x, int p_y) const;
	int getMoveNumber() const;
	bool isViableMove(const Move& move) const;		// move is in form "old Position"-"new Position"  (e3-e4)
	std::list<Move> getValidMoves(int _startX, int _startY) const;
	bool applyMove(const Move& move, bool validate);

private:
	Move parseMove(std::string p_Move);

};

