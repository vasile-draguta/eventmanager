#include "UI.h"
#include "../repository/RepositoryException.h"
#include "../service/EventValidator.h"
#include <iostream>
#include <limits>

UI::UI(const Service &service) : service(service) {}

UI::~UI() = default;

void UI::addEvent() {
    std::string title;
    std::string description;
    std::string dateTime;
    int numberOfParticipants;
    std::string link;

    std::cout << "Enter the title: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);

    std::cout << "Enter the description: ";
    std::getline(std::cin, description);

    std::cout << "Enter the date and time(YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, dateTime);

    std::cout << "Enter the number of participants: ";
    std::cin >> numberOfParticipants;

    std::cout << "Enter the link: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, link);

    Event e{title, description, dateTime, numberOfParticipants, link};

    try {
        service.addEvent(e);
        std::cout << "Event added successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::deleteEvent() {
    std::string title;
    std::cout << "Enter the title of the event you want to delete: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    try {
        service.removeEvent(title);
        std::cout << "Event deleted successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
}

void UI::updateEventTitle() {
    std::string eventTitle, newTitle;
    std::cout << "Enter the title of the event you want to update: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, eventTitle);
    std::cout << "Enter the new title: ";
    std::getline(std::cin, newTitle);
    try {
        service.updateEventTitle(eventTitle, newTitle);
        std::cout << "Title updated successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::updateEventDescription() {
    std::string newDescription;
    std::string title;
    std::cout << "Enter the title of the event you want to update: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter the new description: ";
    std::getline(std::cin, newDescription);

    try {
        service.updateEventDescription(title, newDescription);
        std::cout << "Description updated successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::updateEventDateTime() {
    std::string newDateTime;
    std::string title;
    std::cout << "Enter the title of the event you want to update: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter the new date and time(YYYY-MM-DD HH:MM:SS): ";
    std::getline(std::cin, newDateTime);

    try {
        service.updateEventDateTime(title, newDateTime);
        std::cout << "Date and time updated successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::updateEventNumberOfParticipants() {
    int newNumberOfParticipants;
    std::string title;
    std::cout << "Enter the title of the event you want to update: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter the new number of participants: ";
    std::cin >> newNumberOfParticipants;
    try {
        service.updateEventNumberOfParticipants(title, newNumberOfParticipants);
        std::cout << "Number of participants updated successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::updateEventLink() {
    std::string newLink;
    std::string title;
    std::cout << "Enter the title of the event you want to update: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    std::cout << "Enter the new link: ";
    std::getline(std::cin, newLink);

    try {
        service.updateEventLink(title, newLink);
        std::cout << "Link updated successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::printEvents(const std::vector<Event> &events) {
    if (events.empty())
        std::cout << "There are no events\n";

    for (const auto &event: events) {
        std::cout << event.ToString() << "\n";
    }
}


void UI::runAdminMenu() {
    bool adminMenuRunning = true;
    int userInput;
    while (adminMenuRunning) {
        std::cout << "Choose an option: \n";
        std::cout << "1. Add event\n";
        std::cout << "2. Delete event\n";
        std::cout << "3. Update event\n";
        std::cout << "4. Display all events\n";
        std::cout << "0. Exit\n";
        std::cin >> userInput;
        if (userInput == 1) {
            addEvent();
        } else if (userInput == 2) {
            deleteEvent();
        } else if (userInput == 3) {
            std::cout << "1. Update title\n";
            std::cout << "2. Update description\n";
            std::cout << "3. Update date and time\n";
            std::cout << "4. Update number of participants\n";
            std::cout << "5. Update link\n";
            std::cin >> userInput;
            if (userInput == 1) {
                updateEventTitle();
            } else if (userInput == 2) {
                updateEventDescription();
            } else if (userInput == 3) {
                updateEventDateTime();
            } else if (userInput == 4) {
                updateEventNumberOfParticipants();
            } else if (userInput == 5) {
                updateEventLink();
            } else {
                std::cout << "Invalid input\n";
            }
        } else if (userInput == 4) {
            std::vector<Event> adminEvents = service.getCopyOfAllEvents();
            printEvents(adminEvents);
        } else if (userInput == 0) {
            adminMenuRunning = false;
        } else {
            std::cout << "Invalid input\n";
        }
    }
}

void UI::displayEventsInUserList() {
    std::vector<Event> userEvents = service.getUserEvents();
    printEvents(userEvents);
}

void UI::addEventToUserList(const Event &event) {
    try {
        service.addEventToUserEvents(event);
        std::cout << "Event added to list successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
    catch (EventException &e) {
        for (const auto &error: e.getErrors()) {
            std::cout << error << "\n";
        }
    }
}

void UI::deleteEventFromUserList() {
    std::string title;
    std::cout << "Enter the title of the event you want to delete: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, title);
    try {
        service.removeEventFromUserEvents(title);
        std::cout << "Event deleted successfully\n";
    }
    catch (RepositoryException &e) {
        std::cout << e.what() << "\n";
    }
}

void UI::runEventListMenu() {
    std::string userInput;
    std::cout << "Input the month you want to see the events for or press Enter to see all events: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, userInput);

    std::vector<Event *> events = service.getEventsByMonth(userInput);
    int eventIndex = 0;
    if (events.empty()) {
        std::cout << "There are no events for this month\n";
    } else {
        bool eventMenuIsRunning = true;
        Event e;
        userInput = "";
        while (eventMenuIsRunning) {
            e = *events[eventIndex];
            std::cout << e.ToString() << "\n";
            std::cout << "1.Next event\n";
            std::cout << "2.Previous event\n";
            std::cout << "3.Open link to event\n";
            std::cout << "4.Add event to list\n";
            std::cout << "0.Go back\n";
            std::cin >> userInput;
            if (userInput == "1") {
                eventIndex == int(events.size()) - 1 ? eventIndex = 0 : eventIndex++;
            } else if (userInput == "2") {
                eventIndex == 0 ? eventIndex = int(events.size()) - 1 : eventIndex--;
            } else if (userInput == "3") {
                Service::openUserEventLink(e.getLink());
            } else if (userInput == "4") {
                addEventToUserList(e);
            } else if (userInput == "0") {
                eventMenuIsRunning = false;
            } else {
                std::cout << "Invalid input\n";
            }
        }
    }
}


void UI::runUserMenu() {
    bool userMenuIsRunning = true;
    std::string userInput;

    while (userMenuIsRunning) {
        std::cout << "1.Display events in list\n";
        std::cout << "2.List events by month\n";
        std::cout << "3.Save list to file\n";
        std::cout << "4.Open list\n";
        std::cout << "0.Exit\n";
        std::cin >> userInput;
        if (userInput == "1") {
            displayEventsInUserList();
            std::cout << "1.Delete event from list\n";
            std::cout << "0.Go back\n";
            std::cin >> userInput;
            if (userInput == "1") {
                deleteEventFromUserList();
            } else if (userInput == "0") {
                continue;
            } else {
                std::cout << "Invalid input\n";
            }
        } else if (userInput == "2") {
            runEventListMenu();
        } else if (userInput == "3") {
            try {
                service.saveUserEventsToFile();
                std::cout << "List saved successfully\n";
            }
            catch (FileException &e) {
                std::cout << e.what() << "\n";
            }
        } else if (userInput == "4") {
            try {
                service.openUserEventsFile();
            }
            catch (FileException &e) {
                std::cout << e.what() << "\n";
            }
        } else if (userInput == "0") {
            userMenuIsRunning = false;
        } else {
            std::cout << "Invalid input\n";
        }
    }
}

void UI::runMenu() {
    bool menuRunning = true;
    int userInput;
    while (menuRunning) {
        std::cout << "Choose the mode of the application: \n";
        std::cout << "1.Administrator mode\n";
        std::cout << "2.User mode\n";
        std::cout << "0.Exit\n";
        std::cin >> userInput;
        if (userInput == 1) {
            std::cout << "Administrator mode\n";
            runAdminMenu();
        } else if (userInput == 2) {
            std::cout << "User mode\n";
            runUserMenu();
        } else if (userInput == 0) {
            menuRunning = false;
        } else {
            std::cout << "Invalid input\n";
        }
    }
}
