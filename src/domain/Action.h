#pragma once

#include "../repository/Repository.h"
#include "../repository/FileRepository.h"
#include "../domain/Event.h"


class Action {
public:
    Action()= default;
    virtual ~Action()= default;

    virtual void executeUndo() = 0;
    virtual void executeRedo() = 0;
};

class AddAction : public Action {
private:
    Repository &repo;
    Event addedEvent;
public:
    AddAction(Repository &repo, const Event &addedEvent) : repo(repo), addedEvent(addedEvent) {}

    void executeUndo() override {
        repo.removeFromRepository(addedEvent.getTitle());
    }

    void executeRedo() override {
        repo.addToRepository(addedEvent);
    }
};

class RemoveAction : public Action {
private:
    Repository &repo;
    Event removedEvent;
public:
    RemoveAction(Repository &repo, const Event &removedEvent) : repo(repo), removedEvent(removedEvent) {}

    void executeUndo() override {
        repo.addToRepository(removedEvent);
    }

    void executeRedo() override {
        repo.removeFromRepository(removedEvent.getTitle());
    }
};

class UpdateAction : public Action {
private:
    Repository &repo;
    Event oldEvent;
    Event newEvent;
public:
    UpdateAction(Repository &repo, const Event &oldEvent, const Event &newEvent) : repo(repo), oldEvent(oldEvent), newEvent(newEvent) {}

    void executeUndo() override {
        repo.removeFromRepository(newEvent.getTitle());
        repo.addToRepository(oldEvent);
    }

    void executeRedo() override {
        repo.removeFromRepository(oldEvent.getTitle());
        repo.addToRepository(newEvent);
    }
};

