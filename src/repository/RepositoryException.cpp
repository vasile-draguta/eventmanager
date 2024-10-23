#include "RepositoryException.h"

RepositoryException::RepositoryException(const std::string &msg) {
    this->message = msg;
}

const char *RepositoryException::what() {
    return this->message.c_str();
}

FileException::FileException(const std::string &msg) {
    this->message = msg;
}

const char *FileException::what() {
    return this->message.c_str();
}
