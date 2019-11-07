#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include "io_manager.hpp"

bool io_manager::readFileToBuffer(std::string &path, std::vector<u_char> &buffer) {
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

bool io_manager::readFileToBuffer(std::string &path, std::vector<char> &buffer) {
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

bool io_manager::readFileIntoCluster(std::string &path, std::vector<classifiers::ClassVector> &cluster) {
    std::ifstream file(path, std::ios::out);

    if (!file.is_open()) {
        std::cerr << path << std::endl;
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string label;
        iss >> label;
        std::vector<double> features((std::istream_iterator<double>(iss)), std::istream_iterator<double>());
        cluster.emplace_back(classifiers::ClassVector(label, features));
    }

    file.close();
    return true;
}
