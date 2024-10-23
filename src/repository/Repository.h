#pragma once

#include "../domain/Event.h"
#include <vector>


class Repository {
protected:
    std::vector<Event> data{};

public:
    // Default constructor
    explicit Repository();

    // Destructor
    virtual ~Repository();

    /*
     * Adds an event to the repository if it is not already in the repository
     * Input: Event - eventToAdd
     * Returns: true if the event was added, false otherwise
     */
    void addToRepository(const Event &eventToAdd);

    /*
     * Removes an event from the repository
     * Input: string - title
     * Returns: true if the event was removed, false otherwise
     */
    void removeFromRepository(const std::string &title);

    /*
     * Returns the size of the repository
     * Returns: int - the size of the repository
     */
    int getRepositorySize();

    /*
     * Returns the event from the repository at a given index
     * Input: int - index
     * Returns: Event
     */
    Event &getEventByIndex(int index);


    /*
     * Returns the event from the repository with a given title
     * Input: string - title
     * Returns: Event if the event was found, throws an exception otherwise
     */
    Event &getEventByTitle(const std::string &title);

    /*
     * Returns a copy of the repository
     * Returns: DynamicArray<Event> - the copy of the repository
     */
    std::vector<Event> getRepositoryCopy();

    /*
     * Saves the repository to a file
     */
    virtual void saveToFile();

    /*
     * Loads the repository from a file
     */
    virtual void readFromFile();

    virtual void openFile();
};


