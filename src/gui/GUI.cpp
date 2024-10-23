#include "GUI.h"
#include "../repository/RepositoryException.h"
#include "../service/EventValidator.h"

GUI::GUI(QApplication &app, Service &service) : app(app), service(service) {
    mainWindow = new QWidget();
    mainWindow->resize(1000, 720);
    mainWindow->setWindowTitle("Event Manager");
    mainWindow->setWindowIcon(QIcon("../icon.jpg"));
    QApplication::setFont(QFont("Helvetica", 14));

    adminEventsModel = new EventsTableModel(service.getCopyOfAllEvents());
    userEventsModel = new EventsTableModel(service.getUserEvents());

    adminEventsTable = new QTableView();
    adminEventsTable->setModel(adminEventsModel);
    userEventsTable = new QTableView();
    userEventsTable->setModel(userEventsModel);
    adminEventsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    userEventsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    adminEventsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    userEventsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    adminEventsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    userEventsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    auto *linkDelegate = new LinkDelegate();
    adminEventsTable->setItemDelegateForColumn(4, linkDelegate);
    userEventsTable->setItemDelegateForColumn(4, linkDelegate);

    auto *undoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z), mainWindow);
    QObject::connect(undoShortcut, &QShortcut::activated, [this] {
        undo();
    });
    auto *redoShortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), mainWindow);
    QObject::connect(redoShortcut, &QShortcut::activated, [this] {
        redo();
    });

    stackedLayout = new QStackedLayout(mainWindow);
    mainWindow->setLayout(stackedLayout);
    mainLayout();
    adminLayout();
    userLayout();
    adminAddEvent();
    adminUpdateEvent();
    userSelectMonth();
    eventsPieChart();
    centerWindow();
}

void GUI::centerWindow() {
    mainWindow->move(QGuiApplication::screens().first()->geometry().center() - mainWindow->rect().center());
}

bool GUI::isEventSelected(QTableView *table) {
    if (table->selectionModel()->selectedRows().empty()) {
        QMessageBox::critical(mainWindow, "Error", "Please select an event to remove!");
        return false;
    }
    return true;
}

void GUI::clearLineEdits(QLineEdit **lineEdits) {
    for (int i = 0; i < 5; i++) {
        lineEdits[i]->clear();
    }
}

void GUI::adminAddEvent() {
    auto *addLabel = new QLabel("Add Event");
    addLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    auto *titleLabel = new QLabel("Title:");
    titleLabel->setFixedWidth(150);
    auto *titleEdit = new QLineEdit();
    titleEdit->setFixedHeight(40);
    auto *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleEdit);

    auto *descriptionLabel = new QLabel("Description:");
    descriptionLabel->setFixedWidth(150);
    auto *descriptionEdit = new QLineEdit();
    descriptionEdit->setFixedHeight(40);
    auto *descriptionLayout = new QHBoxLayout();
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionEdit);

    auto *dateTimeLabel = new QLabel("Date and Time:");
    dateTimeLabel->setFixedWidth(150);
    auto *dateTimeEdit = new QLineEdit();
    dateTimeEdit->setFixedHeight(40);
    auto *dateTimeLayout = new QHBoxLayout();
    dateTimeLayout->addWidget(dateTimeLabel);
    dateTimeLayout->addWidget(dateTimeEdit);

    auto *participantsLabel = new QLabel("Participants:");
    participantsLabel->setFixedWidth(150);
    auto *participantsEdit = new QLineEdit();
    participantsEdit->setFixedHeight(40);
    auto *participantsLayout = new QHBoxLayout();
    participantsLayout->addWidget(participantsLabel);
    participantsLayout->addWidget(participantsEdit);

    auto *linkLabel = new QLabel("Link:");
    linkLabel->setFixedWidth(150);
    auto *linkEdit = new QLineEdit();
    linkEdit->setFixedHeight(40);
    auto *linkLayout = new QHBoxLayout();
    linkLayout->addWidget(linkLabel);
    linkLayout->addWidget(linkEdit);

    auto *addButton = new QPushButton("Add");
    auto *goBackButton = new QPushButton("Go Back");

    QObject::connect(addButton, &QPushButton::clicked,
                     [this, titleEdit, descriptionEdit, dateTimeEdit, participantsEdit, linkEdit] {
                         QString name = titleEdit->text();
                         QString description = descriptionEdit->text();
                         QString dateTime = dateTimeEdit->text();
                         int participants = participantsEdit->text().toInt();
                         QString link = linkEdit->text();
                         Event event = Event(name.toStdString(), description.toStdString(), dateTime.toStdString(),
                                             participants, link.toStdString());
                         try {
                             service.addEvent(event);
                             QMessageBox::information(mainWindow, "Success", "Event added successfully!");
                             changeWindowLayout(ADMIN_LAYOUT);
                             QLineEdit *lineEdits[] = {titleEdit, descriptionEdit, dateTimeEdit, participantsEdit,
                                                       linkEdit};
                             adminEventsModel->updateEvents(service.getCopyOfAllEvents());
                             clearLineEdits(lineEdits);
                         } catch (RepositoryException &e) {
                             QMessageBox::critical(mainWindow, "Error", e.what());
                         }
                         catch (EventException &e) {
                             std::vector<std::string> errors = e.getErrors();
                             std::string text;
                             for (const auto &error: errors) {
                                 text += error + "\n";
                             }
                             QMessageBox::critical(mainWindow, "Error", QString::fromStdString(text));
                         }
                     });
    QObject::connect(goBackButton, &QPushButton::clicked,
                     [this, titleEdit, descriptionEdit, dateTimeEdit, participantsEdit, linkEdit] {
                         QLineEdit *lineEdits[] = {titleEdit, descriptionEdit, dateTimeEdit, participantsEdit,
                                                   linkEdit};
                         clearLineEdits(lineEdits);
                         changeWindowLayout(ADMIN_LAYOUT);
                     });

    auto *addEventLayout = new QGridLayout();
    addEventLayout->addWidget(addLabel, 0, 0, 1, 2, Qt::AlignCenter);
    addEventLayout->addLayout(titleLayout, 1, 0);
    addEventLayout->addLayout(descriptionLayout, 2, 0);
    addEventLayout->addLayout(dateTimeLayout, 3, 0);
    addEventLayout->addLayout(participantsLayout, 4, 0);
    addEventLayout->addLayout(linkLayout, 5, 0);
    addEventLayout->addWidget(addButton, 6, 0);
    addEventLayout->addWidget(goBackButton, 7, 0);
    auto *addEventWidget = new QWidget(mainWindow);
    addEventWidget->setLayout(addEventLayout);
    stackedLayout->addWidget(addEventWidget);
}

void GUI::adminRemoveEvent() {
    try {
        std::string eventTitle = adminEventsTable->selectionModel()->selectedRows().at(0).data().toString().toStdString();
        service.removeEvent(eventTitle);
        QMessageBox::information(mainWindow, "Success", "Event removed successfully!");
        adminEventsModel->updateEvents(service.getCopyOfAllEvents());
    }
    catch (RepositoryException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
}

void GUI::setUpdateLineEdits() {
    std::string eventTitle = adminEventsTable->selectionModel()->selectedRows().at(0).data().toString().toStdString();
    Event *event = service.getEventByTitle(eventTitle);
    updateLineEdits[0]->setText(QString::fromStdString(event->getTitle()));
    updateLineEdits[1]->setText(QString::fromStdString(event->getDescription()));
    updateLineEdits[2]->setText(QString::fromStdString(event->getDateTime()));
    updateLineEdits[3]->setText(QString::number(event->getNumberOfParticipants()));
    updateLineEdits[4]->setText(QString::fromStdString(event->getLink()));
}

void GUI::adminUpdateEvent() {
    auto *label = new QLabel("Update Event");
    label->setStyleSheet("font-size: 24px; font-weight: bold;");

    auto *titleLabel = new QLabel("Title:");
    titleLabel->setFixedWidth(150);
    auto *titleEdit = new QLineEdit();
    titleEdit->setFixedHeight(40);
    auto *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleEdit);

    auto *descriptionLabel = new QLabel("Description:");
    descriptionLabel->setFixedWidth(150);
    auto *descriptionEdit = new QLineEdit();
    descriptionEdit->setFixedHeight(40);
    auto *descriptionLayout = new QHBoxLayout();
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionEdit);

    auto *dateTimeLabel = new QLabel("Date and Time:");
    dateTimeLabel->setFixedWidth(150);
    auto *dateTimeEdit = new QLineEdit();
    dateTimeEdit->setFixedHeight(40);
    auto *dateTimeLayout = new QHBoxLayout();
    dateTimeLayout->addWidget(dateTimeLabel);
    dateTimeLayout->addWidget(dateTimeEdit);

    auto *participantsLabel = new QLabel("Participants:");
    participantsLabel->setFixedWidth(150);
    auto *participantsEdit = new QLineEdit();
    participantsEdit->setFixedHeight(40);
    auto *participantsLayout = new QHBoxLayout();
    participantsLayout->addWidget(participantsLabel);
    participantsLayout->addWidget(participantsEdit);

    auto *linkLabel = new QLabel("Link:");
    linkLabel->setFixedWidth(150);
    auto *linkEdit = new QLineEdit();
    linkEdit->setFixedHeight(40);
    auto *linkLayout = new QHBoxLayout();
    linkLayout->addWidget(linkLabel);
    linkLayout->addWidget(linkEdit);

    auto *updateButton = new QPushButton("Update");
    auto *goBackButton = new QPushButton("Go Back");

    QObject::connect(updateButton, &QPushButton::clicked,
                     [this, titleEdit, descriptionEdit, dateTimeEdit, participantsEdit, linkEdit] {

                         std::string eventTitle = adminEventsTable->selectionModel()->selectedRows().at(0).data().toString().toStdString();
                         Event *event = service.getEventByTitle(eventTitle);
                         QString name = titleEdit->text();
                         QString description = descriptionEdit->text();
                         QString dateTime = dateTimeEdit->text();
                         int participants = participantsEdit->text().toInt();
                         QString link = linkEdit->text();

                         if (name.toStdString() == event->getTitle() &&
                             description.toStdString() == event->getDescription() &&
                             dateTime.toStdString() == event->getDateTime() &&
                             participants == event->getNumberOfParticipants() &&
                             link.toStdString() == event->getLink()) {
                             QMessageBox::information(mainWindow, "Info", "No changes were made!");
                             return;
                         }

                         Event newEvent = Event(name.toStdString(), description.toStdString(), dateTime.toStdString(),
                                                participants, link.toStdString());

                         try {
                             EventValidator::validateEvent(newEvent);
                         }
                         catch (EventException &e) {
                             std::vector<std::string> errors = e.getErrors();
                             std::string text;
                             for (const auto &error: errors) {
                                 text += error + "\n";
                             }
                             QMessageBox::critical(mainWindow, "Error", QString::fromStdString(text));
                             return;
                         }

                            service.updateEvent(eventTitle, [&newEvent](Event &event) {
                             event = newEvent;
                            });

                         QMessageBox::information(mainWindow, "Success", "Event updated successfully!");
                         adminEventsModel->updateEvents(service.getCopyOfAllEvents());
                         changeWindowLayout(ADMIN_LAYOUT);
                         QLineEdit *lineEdits[] = {titleEdit, descriptionEdit, dateTimeEdit, participantsEdit,
                                                   linkEdit};
                         clearLineEdits(lineEdits);

                     });

    QObject::connect(goBackButton, &QPushButton::clicked,
                     [this, titleEdit, descriptionEdit, dateTimeEdit, participantsEdit, linkEdit] {
                         QLineEdit *lineEdits[] = {titleEdit, descriptionEdit, dateTimeEdit, participantsEdit,
                                                   linkEdit};
                         clearLineEdits(lineEdits);
                         changeWindowLayout(ADMIN_LAYOUT);
                     });

    auto *updateEventLayout = new QGridLayout();
    updateEventLayout->addWidget(label, 0, 0, 1, 2, Qt::AlignCenter);
    updateEventLayout->addLayout(titleLayout, 1, 0);
    updateEventLayout->addLayout(descriptionLayout,2, 0);
    updateEventLayout->addLayout(dateTimeLayout,3, 0);
    updateEventLayout->addLayout(participantsLayout,4, 0);
    updateEventLayout->addLayout(linkLayout,5, 0);
    updateEventLayout->addWidget(updateButton,6, 0);
    updateEventLayout->addWidget(goBackButton,7, 0);
    auto *updateEventWidget = new QWidget(mainWindow);
    updateEventWidget->setLayout(updateEventLayout);
    updateLineEdits[0] = titleEdit;
    updateLineEdits[1] = descriptionEdit;
    updateLineEdits[2] = dateTimeEdit;
    updateLineEdits[3] = participantsEdit;
    updateLineEdits[4] = linkEdit;
    stackedLayout->addWidget(updateEventWidget);
}

void GUI::adminLayout() {
    auto *addButton = new QPushButton("Add Event");
    auto *removeButton = new QPushButton("Remove Event");
    auto *updateButton = new QPushButton("Update Event");
    auto *pieChartButton = new QPushButton("Events - Participants Pie Chart");
    auto *undoButton = new QPushButton("Undo");
    auto *redoButton = new QPushButton("Redo");
    auto *goBackButton = new QPushButton("Go Back");

    QObject::connect(addButton, &QPushButton::clicked, [this] {
        changeWindowLayout(ADD_ADMIN_EVENT);
    });
    QObject::connect(removeButton, &QPushButton::clicked, [this] {
        if (!isEventSelected(adminEventsTable))
            return;
        adminRemoveEvent();
    });
    QObject::connect(updateButton, &QPushButton::clicked, [this] {
        if (!isEventSelected(adminEventsTable))
            return;
        setUpdateLineEdits();
        changeWindowLayout(UPDATE_ADMIN_EVENT);
    });
    QObject::connect(pieChartButton, &QPushButton::clicked, [this] {
        if (service.getCopyOfAllEvents().empty()) {
            QMessageBox::information(mainWindow, "Info", "No events to display!");
            return;
        }

        if (pieChartWidget != nullptr) {
            stackedLayout->removeWidget(pieChartWidget);
            delete pieChartWidget;
        }

        pieChartWidget = eventsPieChart();
        stackedLayout->addWidget(pieChartWidget);
        changeWindowLayout(PIE_CHART);
    });
    QObject::connect(goBackButton, &QPushButton::clicked, [this] {
        changeWindowLayout(MAIN_LAYOUT);
    });

    QObject::connect(undoButton, &QPushButton::clicked, [this] {
        undo();
    });

    QObject::connect(redoButton, &QPushButton::clicked, [this] {
        redo();
    });

    auto *adminLayout = new QVBoxLayout();
    adminLayout->addWidget(adminEventsTable);
    adminLayout->addWidget(addButton);
    adminLayout->addWidget(removeButton);
    adminLayout->addWidget(updateButton);
    adminLayout->addWidget(undoButton);
    adminLayout->addWidget(redoButton);
    adminLayout->addWidget(pieChartButton);
    adminLayout->addWidget(goBackButton);

    auto *adminWidget = new QWidget(mainWindow);
    adminWidget->setLayout(adminLayout);
    stackedLayout->addWidget(adminWidget);
}

void GUI::userRemoveEvent() {
    try {
        std::string eventTitle = userEventsTable->selectionModel()->selectedRows().at(0).data().toString().toStdString();
        service.removeEventFromUserEvents(eventTitle);
        QMessageBox::information(mainWindow, "Success", "Event removed successfully!");
        userEventsModel->updateEvents(service.getUserEvents());
    }
    catch (RepositoryException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
    catch (FileException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
}

void GUI::userOpenEvents() {
    if (service.getUserEvents().empty()) {
        QMessageBox::information(mainWindow, "Info", "No events to open!");
        return;
    }

    try {
        service.openUserEventsFile();
    }
    catch (RepositoryException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
}

void GUI::userSelectMonth() {
    auto *label = new QLabel("Select a month to list events:");
    label->setStyleSheet("font-size: 20px; font-weight: bold;");
    auto *monthComboBox = new QComboBox();
    auto *listButton = new QPushButton("List Events");
    auto *goBackButton = new QPushButton("Go Back");

    QStringList months = {"All months", "January", "February", "March", "April", "May", "June",
                          "July", "August", "September", "October", "November", "December"};

    monthComboBox->addItems(months);

    QObject::connect(listButton, &QPushButton::clicked, [this, monthComboBox] {
        std::string month = monthComboBox->currentText().toStdString();
        std::vector<Event *> events;
        if (month == "All months")
            events = service.getEventsByMonth("");
        else
            events = service.getEventsByMonth(month);

        if (events.empty()) {
            QMessageBox::information(mainWindow, "Info", "No events found!");
            return;
        }

        QWidget *listEventsWidget = userListEvents(events);
        stackedLayout->addWidget(listEventsWidget);
        changeWindowLayout(LIST_USER_EVENTS);

    });
    QObject::connect(goBackButton, &QPushButton::clicked, [this] {
        changeWindowLayout(USER_LAYOUT);
    });

    auto *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->setAlignment(label, Qt::AlignCenter);
    layout->addWidget(monthComboBox);
    layout->addWidget(listButton);
    layout->addWidget(goBackButton);

    auto *widget = new QWidget;
    widget->setLayout(layout);
    stackedLayout->addWidget(widget);
}

QWidget *GUI::userListEvents(std::vector<Event *> eventsByMonth) {
    auto *label = new QLabel("List of Events");
    label->setStyleSheet("font-size: 24px; font-weight: bold;");

    auto *titleLabel = new QLabel("Title:");
    auto *titleEdit = new QLineEdit();
    titleLabel->setFixedWidth(150);
    titleEdit->setReadOnly(true);
    auto *titleLayout = new QHBoxLayout();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleEdit);

    auto *descriptionLabel = new QLabel("Description:");
    auto *descriptionEdit = new QLineEdit();
    descriptionLabel->setFixedWidth(150);
    descriptionEdit->setReadOnly(true);
    auto *descriptionLayout = new QHBoxLayout();
    descriptionLayout->addWidget(descriptionLabel);
    descriptionLayout->addWidget(descriptionEdit);

    auto *dateTimeLabel = new QLabel("Date and Time:");
    auto *dateTimeEdit = new QLineEdit();
    dateTimeLabel->setFixedWidth(150);
    dateTimeEdit->setReadOnly(true);
    auto *dateTimeLayout = new QHBoxLayout();
    dateTimeLayout->addWidget(dateTimeLabel);
    dateTimeLayout->addWidget(dateTimeEdit);

    auto *participantsLabel = new QLabel("Participants:");
    auto *participantsEdit = new QLineEdit();
    participantsLabel->setFixedWidth(150);
    participantsEdit->setReadOnly(true);
    auto *participantsLayout = new QHBoxLayout();
    participantsLayout->addWidget(participantsLabel);
    participantsLayout->addWidget(participantsEdit);

    auto *linkLabel = new QLabel("Link:");
    auto *linkEdit = new QLineEdit();
    linkLabel->setFixedWidth(150);
    linkEdit->setReadOnly(true);
    auto *linkLayout = new QHBoxLayout();
    linkLayout->addWidget(linkLabel);
    linkLayout->addWidget(linkEdit);

    auto *nextButton = new QPushButton(">");
    auto *previousButton = new QPushButton("<");
    auto *addButton = new QPushButton("Add Event");
    auto *goBackButton = new QPushButton("Go Back");
    auto *buttonsLayout = new QGridLayout();
    buttonsLayout->addWidget(previousButton, 0, 0);
    buttonsLayout->addWidget(nextButton, 0, 1);
    buttonsLayout->addWidget(addButton, 1, 0, 1, 2);
    buttonsLayout->addWidget(goBackButton, 2, 0, 1, 2);

    auto updateFields = [titleEdit, descriptionEdit, dateTimeEdit, participantsEdit, linkEdit](Event *event) {
        titleEdit->setText(QString::fromStdString(event->getTitle()));
        descriptionEdit->setText(QString::fromStdString(event->getDescription()));
        dateTimeEdit->setText(QString::fromStdString(event->getDateTime()));
        participantsEdit->setText(QString::number(event->getNumberOfParticipants()));
        linkEdit->setText(QString::fromStdString(event->getLink()));
    };

    auto index = std::make_shared<int>(0);
    updateFields(eventsByMonth[*index]);

    QObject::connect(nextButton, &QPushButton::clicked, [eventsByMonth, updateFields, index] {
        if (*index == eventsByMonth.size() - 1)
            *index = 0;
        else
            *index = (*index + 1);
        updateFields(eventsByMonth[*index]);
    });

    QObject::connect(previousButton, &QPushButton::clicked, [eventsByMonth, updateFields, index] {
        if (*index == 0)
            *index = eventsByMonth.size() - 1;
        else
            *index = (*index - 1);
        updateFields(eventsByMonth[*index]);
    });

    QObject::connect(addButton, &QPushButton::clicked, [this, index, eventsByMonth, updateFields] {
        Event *event = eventsByMonth[*index];
        try {
            service.addEventToUserEvents(*event);
            if (*index == eventsByMonth.size() - 1)
                *index = 0;
            else
                *index = (*index + 1);
            QMessageBox::information(mainWindow, "Success", "Event added successfully!");
            userEventsModel->updateEvents(service.getUserEvents());
            updateFields(eventsByMonth[*index]);
        }
        catch (RepositoryException &e) {
            QMessageBox::critical(mainWindow, "Error", e.what());
        }
        catch (FileException &e) {
            QMessageBox::critical(mainWindow, "Error", e.what());
        }
    });

    QObject::connect(goBackButton, &QPushButton::clicked, [this] {
        int currentIndex = stackedLayout->currentIndex();
        QWidget *currentWidget = stackedLayout->widget(currentIndex);
        stackedLayout->removeWidget(currentWidget);
        delete currentWidget;
        changeWindowLayout(USER_LAYOUT);
    });

    auto *userListEventsLayout = new QVBoxLayout();
    userListEventsLayout->addWidget(label);
    userListEventsLayout->setAlignment(label, Qt::AlignCenter);
    userListEventsLayout->addLayout(titleLayout);
    userListEventsLayout->addLayout(descriptionLayout);
    userListEventsLayout->addLayout(dateTimeLayout);
    userListEventsLayout->addLayout(participantsLayout);
    userListEventsLayout->addLayout(linkLayout);
    userListEventsLayout->addLayout(buttonsLayout);

    auto *userListEventsWidget = new QWidget;
    userListEventsWidget->setLayout(userListEventsLayout);
    return userListEventsWidget;
}

void GUI::userLayout() {
    auto *removeButton = new QPushButton("Remove Event");
    auto *listEventsByMonth = new QPushButton("List Events by Month");
    auto *openEventsFromFileButton = new QPushButton("Open Events from File");
    auto *undoButton = new QPushButton("Undo");
    auto *redoButton = new QPushButton("Redo");
    auto *goBackButton = new QPushButton("Go Back");

    QObject::connect(removeButton, &QPushButton::clicked, [this] {
        if (!isEventSelected(userEventsTable))
            return;
        userRemoveEvent();
    });
    QObject::connect(listEventsByMonth, &QPushButton::clicked, [this] {
        changeWindowLayout(USER_SELECT_MONTH);
    });
    QObject::connect(openEventsFromFileButton, &QPushButton::clicked, [this] {
        userOpenEvents();
    });
    QObject::connect(goBackButton, &QPushButton::clicked, [this] {
        changeWindowLayout(MAIN_LAYOUT);
    });

    QObject::connect(undoButton, &QPushButton::clicked, [this] {
        undo();
    });

    QObject::connect(redoButton, &QPushButton::clicked, [this] {
        redo();
    });

    auto *userLayout = new QVBoxLayout();
    userLayout->addWidget(userEventsTable);
    userLayout->addWidget(removeButton);
    userLayout->addWidget(listEventsByMonth);
    userLayout->addWidget(openEventsFromFileButton);
    userLayout->addWidget(undoButton);
    userLayout->addWidget(redoButton);
    userLayout->addWidget(goBackButton);

    auto *userWidget = new QWidget(mainWindow);
    userWidget->setLayout(userLayout);
    stackedLayout->addWidget(userWidget);
}

QWidget *GUI::eventsPieChart() {
    std::vector<Event> eventsByDate = service.getCopyOfAllEvents();
    auto *series = new QPieSeries();
    for (const auto &event: eventsByDate) {
        QString label =
                QString::fromStdString(event.getTitle()) + " (" + QString::number(event.getNumberOfParticipants()) +
                ")";
        series->append(label, event.getNumberOfParticipants());
    }

    series->setLabelsVisible(true);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Events by Participants");

    chart->legend()->hide();
    chart->setAnimationOptions(QChart::SeriesAnimations);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto *goBackButton = new QPushButton("Go Back");
    QObject::connect(goBackButton, &QPushButton::clicked, [this] {
        changeWindowLayout(ADMIN_LAYOUT);
    });

    auto *layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->addWidget(goBackButton);

    auto *widget = new QWidget();
    widget->setLayout(layout);
    return widget;
}

void GUI::mainLayout() {
    auto *introMessage = new QLabel("Welcome to the Event Manager!\n        Please select your role:");
    introMessage->setStyleSheet("font-size: 20px; font-weight: bold;");

    auto *adminButton = new QPushButton("Admin");
    auto *userButton = new QPushButton("User");

    QObject::connect(adminButton, &QPushButton::clicked, [this] {
        changeWindowLayout(ADMIN_LAYOUT);
    });
    QObject::connect(userButton, &QPushButton::clicked, [this] {
        changeWindowLayout(USER_LAYOUT);
    });
    auto *layout = new QVBoxLayout();
    layout->addWidget(introMessage);
    layout->addWidget(adminButton);
    layout->addWidget(userButton);

    layout->setAlignment(introMessage, Qt::AlignCenter);

    auto *mainWidget = new QWidget(mainWindow);
    mainWidget->setLayout(layout);
    stackedLayout->addWidget(mainWidget);
}

void GUI::start() {
    mainWindow->show();
}

int GUI::exec() {
    return QApplication::exec();
}

void GUI::changeWindowLayout(int index) {
    stackedLayout->setCurrentIndex(index);
}

void GUI::undo() {
    try {
        service.undo();
        adminEventsModel->updateEvents(service.getCopyOfAllEvents());
        userEventsModel->updateEvents(service.getUserEvents());
    }
    catch (RepositoryException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
}

void GUI::redo() {
    try {
        service.redo();
        adminEventsModel->updateEvents(service.getCopyOfAllEvents());
        userEventsModel->updateEvents(service.getUserEvents());
    }
    catch (RepositoryException &e) {
        QMessageBox::critical(mainWindow, "Error", e.what());
    }
}


GUI::~GUI() = default;
