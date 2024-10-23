#pragma once

#include <vector>
#include <string>
#include <ctime>
#include <iostream>

class Event {
private:
    int numberOfParticipants{};
    std::string title, description, dateTime, link;

public:
    //Constructor default
    Event();

    //Constructor with parameters
    Event(const std::string &title, const std::string &description, const std::string &dateTime,
          int numberOfParticipants, const std::string &link);

    //Copy constructor
    Event(const Event &event);

    //Destructor
    ~Event();

    //Getters
    [[nodiscard]] std::string getTitle() const;

    [[maybe_unused]] [[nodiscard]] std::string getDescription() const;

    [[nodiscard]] std::string getLink() const;

    [[nodiscard]] int getNumberOfParticipants() const;

    [[nodiscard]] std::string getDateTime() const;

    //Setters
    void setTitle(const std::string &newTitle);

    void setDescription(const std::string &newDescription);

    void setLink(const std::string &newLink);

    void setNumberOfParticipants(int newNumberOfParticipants);

    void setDateTime(const std::string &newDateTime);

    /*
     * Function that returns a string representation of the event in the format: "Title | Description | DateTime | Number of participants | Link"
     *  Returns: string - the string representation of the event
     */
    [[nodiscard]] std::string ToString() const;

    /*
     * Overloading the equality operator
    *  Returns: bool - true if the events have the same title, false otherwise
    */
    bool operator==(const Event &event) const;

    friend std::ostream &operator<<(std::ostream &output, const Event &event);

    friend std::istream &operator>>(std::istream &input, Event &event);
};


