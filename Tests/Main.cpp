#include <catch_amalgamated.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    int result = Catch::Session().run(argc, argv);
    std::cin.get();
    return 0;
}