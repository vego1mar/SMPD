#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <sys/stat.h>
#include "io_manager.hpp"

namespace io_manager {
    bool readFileToBuffer(std::string &path, std::vector<u_char> &buffer) {
        std::ifstream file(path, std::ios::out);

        if (file.fail()) {
            perror(path.c_str());
            return false;
        }

        // Reduce file size by header bytes
        file.seekg(0, std::ios::end);
        int fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        fileSize -= file.tellg();
        buffer.resize(fileSize);

        file.read((char *) &(buffer[0]), fileSize);
        file.close();
        return true;
    }

    bool readFileToBuffer(std::string &path, std::vector<char> &buffer) {
        std::ifstream file(path, std::ios::out);

        if (file.fail()) {
            perror(path.c_str());
            return false;
        }

        std::vector<char> data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        buffer = data;
        file.close();
        return true;
    }

    bool readFileIntoCluster(std::string &path, Cluster &cluster) {
        std::ifstream file(path, std::ios::out);

        if (!file.is_open()) {
            std::cerr << path << std::endl;
            return false;
        }

        std::string line;
        using casing::ClassVector;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string label;
            iss >> label;
            std::vector<double> features((std::istream_iterator<double>(iss)), std::istream_iterator<double>());
            cluster.emplace_back(ClassVector(label, features));
        }

        file.close();
        return true;
    }

    long getFileSize(std::string &fileName) {
        struct stat stat_buffer{};
        int rc = stat(fileName.c_str(), &stat_buffer);
        return rc == 0 ? stat_buffer.st_size : -1;
    }

    bool writeResultsIntoFile(std::string &path, std::vector<NNResultSet> &nnResultSet) {
        std::ofstream file;
        file.open(path, std::ios::out);

        if (!file.is_open()) {
            std::cerr << path << std::endl;
            return false;
        }

        for (const auto &resultSet : nnResultSet) {
            file << resultSet.toString() << std::endl;
        }

        file.close();
        return true;
    }

    bool writeResultsIntoFile(std::string &path, std::vector<std::string> &labels) {
        std::ofstream file;
        file.open(path, std::ios::out);

        if (!file.is_open()) {
            std::cerr << path << std::endl;
            return false;
        }

        for (const auto &label : labels) {
            file << label << std::endl;
        }

        file.close();
        return true;
    }

    bool writeResultsIntoFile(std::string &path, std::vector<Cluster> &subClusters) {
        std::ofstream file;
        file.open(path, std::ios::out);

        if (!file.is_open()) {
            std::cerr << path << std::endl;
            return false;
        }

        for (const auto &cluster : subClusters) {
            for (const auto &group : cluster) {
                file << group.toString() << std::endl;
            }
        }

        file.close();
        return true;
    }

}
