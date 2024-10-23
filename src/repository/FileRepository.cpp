#include "FileRepository.h"
#include <fstream>
#include "RepositoryException.h"

FileRepository::FileRepository() : Repository() {
    filename = "../src/events.txt";
}

void FileRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open())
        throw FileException("Could not open file!");
    for (const auto &event: data) {
        file << event << "\n";
    }
    file.close();
}

void FileRepository::readFromFile() {
    std::ifstream file(filename);
    if (!file.is_open())
        throw FileException("Could not open file!");
    Event event;
    while (file >> event) {
        data.push_back(event);
    }
    file.close();
}

FileRepository::~FileRepository() = default;


CSVRepository::CSVRepository() : Repository() {
    filename = "..src/userEvents.csv";
}

CSVRepository::~CSVRepository() = default;

void CSVRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open())
        throw FileException("Could not open file!");
    for (const auto &event: data) {
        file << event << "\n";
    }
    file.close();
}

void CSVRepository::openFile() {
    std::string command = "notepad " + filename;
    system(command.c_str());
}


HTMLRepository::HTMLRepository() : Repository() {
    filename = "../src/userEvents.html";
}

HTMLRepository::~HTMLRepository() = default;

void HTMLRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open())
        throw FileException("Could not open file!");
    file << "<!DOCTYPE html>\n";
    file << "<html>\n";
    file << "<head>\n";
    file << "<title>Events</title>\n";
    file << "</head>\n";
    file << "<body>\n";
    file << "<table border=\"1\">\n";
    file << "<tr>\n";
    file << "<td>Title</td>\n";
    file << "<td>Description</td>\n";
    file << "<td>Date and Time</td>\n";
    file << "<td>Participants</td>\n";
    file << "<td>Link</td>\n";
    file << "</tr>\n";

    for (const auto &event: data) {
        file << "<tr>\n";
        file << "<td>" << event.getTitle() << "</td>\n";
        file << "<td>" << event.getDescription() << "</td>\n";
        file << "<td>" << event.getDateTime() << "</td>\n";
        file << "<td>" << event.getNumberOfParticipants() << "</td>\n";
        file << "<td>" << event.getLink() << "</td>\n";
        file << "</tr>\n";
    }
    file << "</table>\n";
    file << "</body>\n";
    file << "</html>\n";
    file.close();
}

void HTMLRepository::openFile() {
    std::string command = "start "" " + filename;
    system(command.c_str());
}

