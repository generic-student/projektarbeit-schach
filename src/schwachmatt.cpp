#include "uci/uci.hpp"

int main(int argc, char const *argv[])
{
    //create an instance of a universal chess interface
    sm::uci::UniversalChessInterface interface;

    //start handling messages
    interface.start();

    return 0;
}
