#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include "../repository/Repository.h"
#include "../repository/FileRepository.h"
#include "../domain/Action.h"

class Service {
private:
    Repository &adminRepo;
    Repository &userRepo;

    std::stack<std::shared_ptr<Action>> undoStack;
    std::stack<std::shared_ptr<Action>> redoStack;

    /*
     * Sorts a given vector of events by date
     * Input: DynamicArray<Event> - events
     * Returns: -
     */
    static void sortEventsByDate(std::vector<Event *> &events);

public:
    // Constructor
    Service(Repository &adminRepo, Repository &userRepo);

    // Destructor
    ~Service();

    /*
     * Add an event to the repository
     * Input: Event e - the event to be added
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is already in the repository
     */
    void addEvent(const Event &e);

    /*
     * Remove an event from the repository
     * Input: string - title
     * Returns: -
     * Throws: RepositoryException - if the event is not found
     */
    void removeEvent(const std::string &title);

    /*
     * Get the number of events in the repository
     * Returns: int - the number of events
     */
    int getNumberOfEvents();

    /*
     * Get all the events from the repository
     * Returns: DynamicArray<Event *> - pointer to the events in the admin repository
     */
    std::vector<Event *> getAllEvents();

    /*
     * Get a copy of all events from the repository
     * Returns: vector<Event> - a copy of all events
     */
    std::vector<Event> getCopyOfAllEvents();

    /*
     * Update the title of an event
     * Input: string - eventTitle, string - newTitle
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is not found
     */
    void updateEventTitle(const std::string &eventTitle, const std::string &newTitle);

    /*
     * Update the description of an event
     * Input: string - eventTitle, string - newDescription
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is not found
     */
    void updateEventDescription(const std::string &eventTitle, const std::string &newDescription);

    /*
     * Update the date and time of an event
     * Input: string - eventTitle, string - newDateTime
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is not found
     */
    void updateEventDateTime(const std::string &eventTitle, const std::string &newDateTime);

    /*
     * Update the number of participants of an event
     * Input: string - eventTitle, int - newNumberOfParticipants
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is not found
     */
    void updateEventNumberOfParticipants(const std::string &eventTitle, int newNumberOfParticipants);

    /*
     * Update the link of an event
     * Input: string - eventTitle, string - newLink
     * Returns: -
     * Throws: EventException - if the event is invalid. RepositoryException - if the event is not found
     */
    void updateEventLink(const std::string &eventTitle, const std::string &newLink);

    /*
     * Get all events on a given month, sorted by date, if no month is given, all events are returned
     * Input: string - month
     * Returns: An array of events pointers - the events on the given month
     */
    std::vector<Event *> getEventsByMonth(const std::string &month);

    /*
     * Gets all the events from the user's repository
     * Returns: DynamicArray<Event> - a copy of the events
     */
    std::vector<Event> getUserEvents();

    /*
     * Add an event to the user's repository
     * Input: Event e - the event to be added
     * Returns: -
     * Throws: EventException - if the event is already in the user's repository
     */
    void addEventToUserEvents(const Event &e);

    /*
     * Remove an event from the user's repository
     * Input: string - eventTitle
     * Returns: -
     * Throws: EventException - if the event is not in the user's repository
     */
    void removeEventFromUserEvents(const std::string &eventTitle);

    /*
     * Check if an event is in the user's repository
     * Input: string - eventTitle
     * Returns: -
     * Throws: EventException - if the event is not in the user's repository
     */
    bool isEventInUserEvents(const std::string &eventTitle);

    /*
     * Opens the link of an event in the default browser
     * Input: string - link
     * Returns: -
     */
    static void openUserEventLink(const std::string &link);

    /*
     * Opens the user events file specifically for the user repository
     * Returns: -
     */
    void openUserEventsFile();

    /**
     * Save the user events to a file
     * Returns: -
     */
    void saveUserEventsToFile();

    Event* getEventByTitle(const std::string& title);

    void undo();

    void redo();

    /*
    * Helper function for updating an event. It searches for the event with the given title and applies the update function to it.
    * Input: string - eventTitle, function<void(Event&)> - updateFunc
    * Returns: true - if the event was updated successfully, false - otherwise
    */
    void updateEvent(const std::string &eventTitle, const std::function<void(Event &)> &updateFunc);
};


