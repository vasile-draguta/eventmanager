#include <sstream>
#include "Event.h"


Event::Event() {
    this->title = "";
    this->description = "";
    this->dateTime = "";
    this->numberOfParticipants = 0;
    this->link = "";
}

Event::Event(const std::string &title, const std::string &description, const std::string &dateTime,
             int numberOfParticipants, const std::string &link) {
    this->title = title;
    this->description = description;
    this->dateTime = dateTime;
    this->numberOfParticipants = numberOfParticipants;
    this->link = link;
}

Event::~Event() = default;

Event::Event(const Event &event) {
    this->title = event.title;
    this->description = event.description;
    this->dateTime = event.dateTime;
    this->numberOfParticipants = event.numberOfParticipants;
    this->link = event.link;
}


std::string Event::getTitle() const {
    return this->title;
}

[[maybe_unused]] std::string Event::getDescription() const {
    return this->description;
}

std::string Event::getLink() const {
    return this->link;
}

std::string Event::getDateTime() const {
    return this->dateTime;
}

int Event::getNumberOfParticipants() const {
    return this->numberOfParticipants;
}


void Event::setTitle(const std::string &newTitle) {
    this->title = newTitle;
}

void Event::setDescription(const std::string &newDescription) {
    this->description = newDescription;
}

void Event::setLink(const std::string &newLink) {
    this->link = newLink;
}

void Event::setNumberOfParticipants(int newNumberOfParticipants) {
    this->numberOfParticipants = newNumberOfParticipants;
}

void Event::setDateTime(const std::string &newDateTime) {
    this->dateTime = newDateTime;
}


std::string Event::ToString() const {
    return title + " | " + description + " | " + dateTime + " | " +
           std::to_string(numberOfParticipants) + " | " + link;
}

bool Event::operator==(const Event &event) const {
    if (this->title == event.title)
        return true;
    return false;
}

std::ostream &operator<<(std::ostream &output, const Event &event) {
    output << event.title << "," << event.description << "," << event.dateTime << "," << event.numberOfParticipants << "," << event.link;
    return output;
}

std::istream &operator>>(std::istream &input, Event &event) {
    std::string line, token;
    std::vector<std::string> tokens;


    std::getline(input, line);
    std::istringstream tokenStream(line);
    while (std::getline(tokenStream, token, ','))
        tokens.push_back(token);

    if (tokens.size() != 5)
        return input;

    event.title = tokens[0];
    event.description = tokens[1];
    event.dateTime = tokens[2];
    event.numberOfParticipants = std::stoi(tokens[3]);
    event.link = tokens[4];
    return input;
}




