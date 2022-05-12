#pragma once
#include <string>
#include <array>
#include "move.hpp"
#include <vector>

namespace sm {

	class Chessposition
	{
		//Variablen
	public :
		enum Player { WHITE = 0, BLACK = 1 };
		static const std::string STARTPOS_FEN;

		explicit Chessposition();
		explicit Chessposition(const std::string& fen);
		explicit Chessposition(const std::array<std::array<char, 8>, 8>& pos);

	private:
		Chessposition::Player m_activePlayer = Player::WHITE;
		unsigned int m_moveNumber = 0;					//Based on White Player (negative Numbers are an advantage for Black, positive advantage for white)
		std::array<std::array<char, 8>, 8> m_position;
		std::array<std::array<int, 8>, 8> m_moveCount;
		Move m_previousMove;
	

		//Methoden
	public:
		void setFEN(std::string p_FEN);
		std::string getFEN() const;

		void setActivePlayer(Chessposition::Player p_id);
		Chessposition::Player getActivePlayer() const;

		const std::array<std::array<int, 8>, 8>& getMoveCount() const;
		const std::array<std::array<char, 8>, 8>& getPosition() const;
		char getType(int row, int column) const;
	
		int getMoveCountPos(int row, int column) const;
		int getMoveNumber() const;
		bool isViableMove(const Move& move, bool checkCaptureTarget = true, bool checkKingSafety = true) const;		// move is in form "old Position"-"new Position"  (e3-e4)
		std::vector<Move> getValidMovesForField(int row, int column, bool checkCaptureTarget = true, bool checkKingSafety = true) const;
		std::vector<Move> getValidMoves(bool checkCaptureTarget = true, bool checkKingSafety = true) const;
		bool applyMove(const Move& move, bool validate);

		bool isKingAttackableInNextMove(Move move) const;
		std::array<std::array<bool, 8>, 8> generateThreatMap() const;
		bool isPatt() const;
		bool isMatt() const;
		
	};

}
