#include "EventValidator.h"
#include <utility>


EventException::EventException(std::vector<std::string> _errors): errors{std::move(_errors)} {}

std::vector<std::string> EventException::getErrors() const {
    return this->errors;
}

void EventValidator::validateEvent(const Event &e) {
    std::vector<std::string> errors;
    if (e.getTitle().size() < 3)
        errors.emplace_back("The title name cannot be less than 3 characters!\n");

    if (e.getDescription().size() < 3)
        errors.emplace_back("The description name cannot be less than 3 characters!\n");

    if (e.getNumberOfParticipants() == 0)
        errors.emplace_back("The number of people cannot be 0!\n");

    if(e.getDateTime().empty())
        errors.emplace_back("The date and time cannot be empty!\n");

    if(e.getDateTime()[4] != '-' || e.getDateTime()[7] != '-' || e.getDateTime()[10] != ' ' || e.getDateTime()[13] != ':' || e.getDateTime()[16] != ':'){
        errors.emplace_back("Invalid date and time format\n");
    }

    for(int i = 0; i < e.getDateTime().size(); i++){
        if(i == 4 || i == 7 || i == 10 || i == 13 || i == 16){
            continue;
        }
        if(!isdigit(e.getDateTime()[i])){
            errors.emplace_back("The date and time must be in the format: YYYY-MM-DD HH:MM:SS!\n");
            break;
        }
    }

    if (e.getLink().find("www") != 0)
        errors.emplace_back("The link must start with 'www'!");

    if (!errors.empty())
        throw EventException(errors);
}

void EventValidator::validateDateTime(const std::string &date) {
    std::vector<std::string> errors;
    if(date.size() != 19 || date[4] != '-' || date[7] != '-' || date[10] != ' ' || date[13] != ':' || date[16] != ':')
        errors.emplace_back("The date and time must be in the format: YYYY-MM-DD HH:MM:SS!\n");

    for(int i = 0; i < date.size(); i++){
        if(i == 4 || i == 7 || i == 10 || i == 13 || i == 16){
            continue;
        }
        if(date.size() == 19 && !isdigit(date[i])) {
            errors.emplace_back("The date and time must be in the format: YYYY-MM-DD HH:MM:SS!\n");
            break;
        }
    }

    if (!errors.empty())
        throw EventException(errors);
}
