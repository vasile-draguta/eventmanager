#pragma once

#include <exception>
#include <string>

class RepositoryException : public std::exception {
private:
    std::string message;
public:
    explicit RepositoryException(const std::string &msg);

    const char *what();
};

class FileException : public std::exception {
private:
    std::string message;
public:
    explicit FileException(const std::string &msg);

    const char *what();
};