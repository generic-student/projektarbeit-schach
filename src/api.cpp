#include <iostream>
#include "tcp_server_async.hpp"



int main(int argc, char const *argv[])
{
    boost::asio::io_context io_context;

    api::io::TcpServerAsync server(io_context, 3000);
    server.run();

    return 0;
}
