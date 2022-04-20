#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>


using boost::asio::ip::tcp;

enum ClientState {
    SENDING,
    RECEIVING
};

bool handle_read(tcp::socket& socket, boost::system::error_code& error) {
    boost::array<char, 128> buf;
    
    size_t len = socket.read_some(boost::asio::buffer(buf), error);

    if (error == boost::asio::error::eof)
        return false; // Connection closed cleanly by peer.
    else if (error)
        throw boost::system::system_error(error); // Some other error.

    std::cout << "response: ";
    std::cout.write(buf.data(), len);
    std::cout << std::endl;
    return true;
}

bool handle_write(tcp::socket& socket, const std::string& message, boost::system::error_code& error) {
    socket.write_some(boost::asio::buffer(message), error);

    if(error) {
        throw boost::system::system_error(error);
    }

    return true;
}

int main(int argc, char *argv[])
{
    ClientState state = ClientState::SENDING;
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);

        tcp::resolver::results_type endpoints =
            resolver.resolve(argv[1], argv[2]);

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            boost::system::error_code error;

            //first send a message and then receive the response
            std::string input;
            std::cout << ">> ";
            std::getline(std::cin, input);
            input.append("\n");

            //send a message
            if(!handle_write(socket, input, error)) break;

            //receive the response
            if(!handle_read(socket, error)) break;
            
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
