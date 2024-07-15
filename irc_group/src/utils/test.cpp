#include <iostream>
#include "../../includes/to_string_.hpp"
#include "../../includes/irc_codes.hpp"

int main()
{
    std::cout << to_string_(static_cast<int>(RPL_WELCOME)) << std::endl;
    return 0;
}
