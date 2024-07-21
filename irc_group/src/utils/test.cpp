#include <iostream>
#include "../../includes/to_string_.hpp"
#include "../../includes/irc_codes.hpp"
#include "../../includes/ston.hpp"

bool is_valid_username(const std::string &username);

int main()
{
    // std::cout << to_string_(static_cast<int>(RPL_WELCOME)) << std::endl;
    // std::cout << ston<int>("001") << std::endl;
    std::cout << is_valid_username("amen@ses-") << std::endl;
    return 0;
}
