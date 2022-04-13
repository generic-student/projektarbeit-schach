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

    std::cout.write(buf.data(), len);
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
        if (argc != 4)
        {
            std::cerr << "Usage: client <host> <port> <message>" << std::endl;
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
            bool stop = false;

            switch(state) {
                case ClientState::SENDING:
                    stop = !handle_write(socket, std::string(argv[3])+"\n", error);
                    state = ClientState::RECEIVING;
                    break;
                case ClientState::RECEIVING:
                    stop = !handle_read(socket, error);
                    state = ClientState::SENDING;
                    break;
            }

            if(stop) {
                break;
            }

            
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
