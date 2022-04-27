#include <boost/process.hpp>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    std::string filename;

    if(argc == 1) {
        std::cout << "usage: client <path-to-schachengine>" << std::endl;
        return 1;
    } else {
        filename = argv[1];
    }

    boost::process::child c(filename, boost::process::std_out > stdout, boost::process::std_in < stdin);

    c.wait();
    

    return 0;
}
