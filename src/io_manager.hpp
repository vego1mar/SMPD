#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

#include <string>
#include <vector>
#include "casing.hpp"

using casing::Cluster;
using casing::NNResultSet;

namespace io_manager {

    bool readFileToBuffer(std::string &path, std::vector<u_char> &buffer);

    bool readFileToBuffer(std::string &path, std::vector<char> &buffer);

    bool readFileIntoCluster(std::string &path, Cluster &cluster);

    long getFileSize(std::string &fileName);

    bool writeResultsIntoFile(std::string &path, std::vector<NNResultSet> &nnResultSet);

    bool writeResultsIntoFile(std::string &path, std::vector<std::string> &labels);

}

#endif //IO_MANAGER_HPP
