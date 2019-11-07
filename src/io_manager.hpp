#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

#include <string>
#include <vector>
#include "classifiers.hpp"

namespace io_manager {

    bool readFileToBuffer(std::string &path, std::vector<u_char> &buffer);

    bool readFileToBuffer(std::string &path, std::vector<char> &buffer);

    bool readFileIntoCluster(std::string &path, std::vector<classifiers::ClassVector> &cluster);

}

#endif //IO_MANAGER_HPP
