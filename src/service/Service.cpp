#include "Service.h"
#include "../repository/RepositoryException.h"
#include <map>
#include <algorithm>
#include "EventValidator.h"


Service::Service(Repository &adminRepo, Repository &userRepo) : adminRepo(adminRepo), userRepo(userRepo) {
    this->undoStack = std::stack<std::shared_ptr<Action>>();
    this->redoStack = std::stack<std::shared_ptr<Action>>();
    this->adminRepo.readFromFile();
}

Service::~Service() = default;

void Service::addEvent(const Event &e) {
    EventValidator::validateEvent(e);
    this->adminRepo.addToRepository(e);
    this->adminRepo.saveToFile();
    std::shared_ptr<AddAction> addAction = std::make_shared<AddAction>(this->adminRepo, e);
    this->undoStack.push(addAction);
}

void Service::removeEvent(const std::string &title) {
    std::shared_ptr<RemoveAction> removeAction = std::make_shared<RemoveAction>(this->adminRepo, this->adminRepo.getEventByTitle(title));
    this->undoStack.push(removeAction);
    if(this->isEventInUserEvents(title)) {
        this->userRepo.removeFromRepository(title);
    }
    this->adminRepo.removeFromRepository(title);
    this->adminRepo.saveToFile();
}

int Service::getNumberOfEvents() {
    return this->adminRepo.getRepositorySize();
}

std::vector<Event *> Service::getAllEvents() {
    std::vector<Event *> events;
    events.reserve(this->adminRepo.getRepositorySize());
    for (int i = 0; i < this->adminRepo.getRepositorySize(); i++) {
        events.push_back(&this->adminRepo.getEventByIndex(i));
    }
    return events;
}

std::vector<Event> Service::getCopyOfAllEvents() {
    return std::vector<Event>(this->adminRepo.getRepositoryCopy());
}

void Service::updateEvent(const std::string &eventTitle, const std::function<void(Event &)> &updateFunc) {
    for (int i = 0; i < this->getNumberOfEvents(); i++) {
        if (this->adminRepo.getEventByIndex(i).getTitle() == eventTitle) {
            Event TestEvent = this->adminRepo.getEventByIndex(i);
            updateFunc(TestEvent);
            EventValidator::validateEvent(TestEvent);
            Event oldEvent = this->adminRepo.getEventByIndex(i);
            updateFunc(this->adminRepo.getEventByIndex(i));
            this->adminRepo.saveToFile();
            std::shared_ptr<UpdateAction> updateAction = std::make_shared<UpdateAction>(this->adminRepo, oldEvent, this->adminRepo.getEventByIndex(i));
            this->undoStack.push(updateAction);
            return;
        }
    }
    throw RepositoryException("Event not found!");
}

void Service::updateEventTitle(const std::string &eventTitle, const std::string &newTitle) {
    updateEvent(eventTitle, [&newTitle](Event &event) { event.setTitle(newTitle); });
}

void Service::updateEventDescription(const std::string &eventTitle, const std::string &newDescription) {
    updateEvent(eventTitle, [&newDescription](Event &event) { event.setDescription(newDescription); });
}

void Service::updateEventDateTime(const std::string &eventTitle, const std::string &newDateTime) {
    EventValidator::validateDateTime(newDateTime);
    updateEvent(eventTitle, [&newDateTime](Event &event) { event.setDateTime(newDateTime); });
}

void Service::updateEventNumberOfParticipants(const std::string &eventTitle, int newNumberOfParticipants) {
    updateEvent(eventTitle, [&newNumberOfParticipants](Event &event) { event.setNumberOfParticipants(newNumberOfParticipants); });
}

void Service::updateEventLink(const std::string &eventTitle, const std::string &newLink) {
    return updateEvent(eventTitle, [&newLink](Event &event) { event.setLink(newLink); });
}


std::vector<Event *> Service::getEventsByMonth(const std::string &month) {
    std::map<std::string, std::string> months = {
            {"January",   "01"},
            {"February",  "02"},
            {"March",     "03"},
            {"April",     "04"},
            {"May",       "05"},
            {"June",      "06"},
            {"July",      "07"},
            {"August",    "08"},
            {"September", "09"},
            {"October",   "10"},
            {"November",  "11"},
            {"December",  "12"}
    };

    std::vector<Event *> events = getAllEvents();
    std::vector<Event *> eventsByMonth;

    for (const auto &event: events) {
        if (month.empty() || event->getDateTime().substr(5, 2) == months[month]) {
            eventsByMonth.push_back(event);
        }
    }

    sortEventsByDate(eventsByMonth);

    return eventsByMonth;
}

void Service::sortEventsByDate(std::vector<Event *> &events) {
    auto comparator = [](Event *e1, Event *e2) {
        return e1->getDateTime() < e2->getDateTime();
    };

    std::sort(events.begin(), events.end(), comparator);
}

std::vector<Event> Service::getUserEvents() {
    return this->userRepo.getRepositoryCopy();
}

bool Service::isEventInUserEvents(const std::string &eventTitle) {
    auto userEvents = this->getUserEvents();
    return std::any_of(userEvents.begin(), userEvents.end(), [&eventTitle](const Event &e) {
        return e.getTitle() == eventTitle;
    });
}

void Service::addEventToUserEvents(const Event &e) {
    if (this->isEventInUserEvents(e.getTitle())) {
        throw RepositoryException("Event already in user's events!");
    }
    int newNumberOfParticipants = this->adminRepo.getEventByTitle(e.getTitle()).getNumberOfParticipants() + 1;
    this->updateEventNumberOfParticipants(e.getTitle(), newNumberOfParticipants);
    this->userRepo.addToRepository(e);
    std::shared_ptr<AddAction> addAction = std::make_shared<AddAction>(this->userRepo, e);
    this->undoStack.push(addAction);
}

void Service::removeEventFromUserEvents(const std::string &eventTitle) {
    if (!this->isEventInUserEvents(eventTitle)) {
        throw RepositoryException("Event not in user's events!");
    }
    try {
        this->userRepo.removeFromRepository(eventTitle);
        int newNumberOfParticipants = this->adminRepo.getEventByTitle(eventTitle).getNumberOfParticipants() - 1;
        this->updateEventNumberOfParticipants(eventTitle, newNumberOfParticipants);
        std::shared_ptr<RemoveAction> removeAction = std::make_shared<RemoveAction>(this->userRepo, this->adminRepo.getEventByTitle(eventTitle));
        this->undoStack.push(removeAction);
    } catch (RepositoryException &e) {
        this->userRepo.removeFromRepository(eventTitle);
    }
    catch (FileException &e) {
        this->userRepo.removeFromRepository(eventTitle);
    }
}

void Service::saveUserEventsToFile() {
    this->userRepo.saveToFile();
}

void Service::openUserEventLink(const std::string &link) {
    std::string command = R"("C:\Program Files\Google\Chrome\Application\chrome.exe" )" + link;
    system(command.c_str());
}

void Service::openUserEventsFile() {
    userRepo.openFile();
}

Event *Service::getEventByTitle(const std::string &title) {
    return &this->adminRepo.getEventByTitle(title);
}

void Service::undo() {
    if (this->undoStack.empty()) {
        throw RepositoryException("No more undos!");
    }
    std::shared_ptr<Action> action = this->undoStack.top();
    action->executeUndo();
    this->redoStack.push(action);
    this->undoStack.pop();
}

void Service::redo() {
    if (this->redoStack.empty()) {
        throw RepositoryException("No more redos!");
    }
    std::shared_ptr<Action> action = this->redoStack.top();
    action->executeRedo();
    this->undoStack.push(action);
    this->redoStack.pop();
}
