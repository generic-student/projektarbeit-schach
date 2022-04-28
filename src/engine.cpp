#include "engine.hpp"

namespace sm
{
    Engine::Engine()
    {
    }
    
    const std::string Engine::getID() const
    {
        return m_id;
    }
    
    const std::string Engine::getAuthor() const
    {
        return m_author;
    }
    
    bool Engine::inDebugMode() const
    {
        return m_debugMode;
    }
    
    void Engine::setDebugMode(bool debug)
    {
        m_debugMode = debug;
    }
    
    bool Engine::isReady() const
    {
        return m_ready;
    }
    
    void Engine::setOption(const std::string& option, const std::string& value)
    {
        throw "Invalid Option";
    }

    float Engine::evaluateBoard(const std::array<std::array<char, 8>, 8>& currentBoard)
    {
        float score = 0.0f;
        // TODO: 
        // King Safety
        // Piece Activity
        // Pawn Structure



        // Variables for Bishop Pair Check
        unsigned short int whiteBishops = 0;
        unsigned short int blackBishops = 0;

        for (int i = 0; i < currentBoard.size(); i++)
        {
            for (int j = 0; j < currentBoard[i].size(); j++)
            {
                switch (currentBoard[i][j])
                {
                case 'p':
                    score += -1.0f;
                    break;
                case 'P':
                    score += 1.0f;
                    break;

                case 'r':
                    score += -5.0f;
                    break;
                case 'R':
                    score += 5.0f;
                    break;

                case 'n':
                    score += -3.25f;
                    break;
                case 'N':
                    score += 3.25f;
                    break;

                case 'q':
                    score += -9.75;
                    break;
                case 'Q':
                    score += 9.75f;
                    break;

                case 'b':
                    score += -3.25f;
                    blackBishops += 1;
                    break;
                case 'B':
                    score += 3.25f;
                    whiteBishops += 1;
                    break;

                default:
                    break;
                }
            }
        }

        if (blackBishops >= 2)
        {
            score += -((int)(blackBishops / 2)) * 0.25f;
        }
        if (whiteBishops >= 2)
        {
            score += ((int)(whiteBishops / 2)) * 0.25f;
        }


        return score;
    }
    
    Chessposition& Engine::getPosition()
    {
        return m_position;
    }
    
    const Chessposition& Engine::getPosition() const
    {
        return m_position;
    }


    
}