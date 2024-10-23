#include "EventsTableModel.h"

EventsTableModel::EventsTableModel(std::vector<Event> events, QObject *parent) : QAbstractTableModel(parent) {
    this->events = std::move(events);
}

int EventsTableModel::rowCount(const QModelIndex &parent) const {
    return static_cast<int>(this->events.size());
}

int EventsTableModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant EventsTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole) {
        int row = index.row();
        int column = index.column();
        Event event = this->events[row];
        switch (column) {
            case 0:
                return QString::fromStdString(event.getTitle());
            case 1:
                return QString::fromStdString(event.getDescription());
            case 2:
                return QString::fromStdString(event.getDateTime());
            case 3:
                return QString::number(event.getNumberOfParticipants());
            case 4:
                return QString::fromStdString(event.getLink());
            default:
                break;
        }
    }
    return {};
}

QVariant EventsTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return "Title";
                case 1:
                    return "Description";
                case 2:
                    return "Date and Time";
                case 3:
                    return "Number of Participants";
                case 4:
                    return "Link";
                default:
                    break;
            }
        }
    }
    return {};
}

void EventsTableModel::setEvents(std::vector<Event> events) {
    this->events = std::move(events);
}

void EventsTableModel::updateEvents(std::vector<Event> newEvents) {
    beginResetModel();
    this->events = std::move(newEvents);
    endResetModel();
}
