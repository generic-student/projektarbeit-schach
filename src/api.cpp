#include <iostream>
#include "tcp_server_async.hpp"



int main(int argc, char const *argv[])
{
    //create an io context
    boost::asio::io_context io_context;

    //create an instance of a tcp server
    sm::io::TcpServerAsync server(io_context, 3000);
    
    //start the server
    server.run();

    return 0;
}
