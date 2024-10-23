#include <stdexcept>
#include <algorithm>
#include "Repository.h"
#include "RepositoryException.h"

Repository::Repository() = default;

Repository::~Repository() = default;

void Repository::addToRepository(const Event &eventToAdd) {
    for (Event &event: data) {
        if (event == eventToAdd)
            throw RepositoryException("Event already exists!");
    }
    data.push_back(eventToAdd);
    saveToFile();
}


void Repository::removeFromRepository(const std::string &title) {
    auto it = std::find_if(data.begin(), data.end(), [&title](const Event &event) {
        return event.getTitle() == title;
    });

    if (it != data.end()) {
        data.erase(it);
        data.shrink_to_fit();
        saveToFile();
    } else {
        throw RepositoryException("Event not found!");
    }
}

int Repository::getRepositorySize() {
    return int(data.size());
}

Event &Repository::getEventByIndex(int index) {
    if (index < 0 || index >= data.size())
        throw RepositoryException("Invalid index!");
    return data[index];
}

std::vector<Event> Repository::getRepositoryCopy() {
    return {data.begin(), data.end()};
}

Event &Repository::getEventByTitle(const std::string &title) {
    for (auto &event: data) {
        if (event.getTitle() == title)
            return event;
    }
    throw RepositoryException("Event not found!");
}

void Repository::saveToFile() {}

void Repository::readFromFile() {}

void Repository::openFile() {}
