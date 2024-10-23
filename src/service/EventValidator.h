#pragma once

#include <vector>
#include <string>
#include "../domain/Event.h"

class EventException : public std::exception {

private:
    std::vector<std::string> errors;

public:
    explicit EventException(std::vector<std::string> _errors);

    [[nodiscard]] std::vector<std::string> getErrors() const;
};

class EventValidator {
public:
    EventValidator() = default;

    static void validateEvent(const Event &e);
    static void validateDateTime(const std::string &date);
};
