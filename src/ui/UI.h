#pragma once

#include "../service/Service.h"

class UI {
private:
    Service service;

    void runAdminMenu();

    void runUserMenu();

    void addEvent();

    void deleteEvent();

    static void printEvents(const std::vector<Event> &events);

    void updateEventTitle();

    void updateEventDescription();

    void updateEventDateTime();

    void updateEventNumberOfParticipants();

    void updateEventLink();

    void displayEventsInUserList();

    void addEventToUserList(const Event &event);

    void deleteEventFromUserList();

    void runEventListMenu();

public:
    explicit UI(const Service &service);

    ~UI();

    void runMenu();
};


