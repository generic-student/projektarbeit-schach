#pragma once
#include <string>
#include <array>

class Chessposition
{
	//Variablen
public :


private:
	unsigned int m_activePlayer;
	unsigned int m_moveNumber;
	float m_positionEvaluation;						//Based on White Player (negative Numbers are an advantage for Black, positive advantage for white)
    std::string m_FENString;
	std::array<std::array<int, 8>, 8> m_Position;



	//Methoden
public:
	std::string getField();
	
	int getActivePlayer();

private:



};


