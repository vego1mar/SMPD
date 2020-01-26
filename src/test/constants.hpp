#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>


namespace test {

    struct Constants {
        const std::string MAPLE_OAK_PATH = "../../files/Maple_Oak.csv";
        const std::size_t MAPLE_OAK_LINES = 784;
        const std::size_t MAPLE_OAK_COLUMNS = 65;
        const std::size_t MAPLE_OAK_FEATURES = MAPLE_OAK_COLUMNS - 1;
    };

}

#endif //CONSTANTS_HPP
