#pragma once

#include "Repository.h"

class FileRepository : public Repository {
private:
    std::string filename;
public:
    explicit FileRepository();

    ~FileRepository() override;

    void saveToFile() override;

    void readFromFile() override;
};

class CSVRepository : public Repository {
private:
    std::string filename;
public:
    explicit CSVRepository();

    ~CSVRepository() override;

    void saveToFile() override;

    void openFile() override;
};

class HTMLRepository : public Repository {
private:
    std::string filename;
public:
    explicit HTMLRepository();

    ~HTMLRepository() override;

    void saveToFile() override;

    void openFile() override;
};