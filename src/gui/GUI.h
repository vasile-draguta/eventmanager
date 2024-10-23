#pragma once

#include <QApplication>
#include <QWindow>
#include <QLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include <QScreen>
#include <QLineEdit>
#include <QStackedLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QtCharts>
#include <QShortcut>
#include <QKeySequence>
#include "../service/Service.h"
#include "../domain/Event.h"
#include "../gui/EventsTableModel.h"

#define MAIN_LAYOUT 0
#define ADMIN_LAYOUT 1
#define USER_LAYOUT 2
#define ADD_ADMIN_EVENT 3
#define UPDATE_ADMIN_EVENT 4
#define USER_SELECT_MONTH 5
#define PIE_CHART 6
#define LIST_USER_EVENTS 7

class GUI : public QMainWindow {
private:
    QApplication &app;
    QWidget *mainWindow;
    QStackedLayout *stackedLayout;
    Service &service;
    QTableView *adminEventsTable;
    QTableView *userEventsTable;
    EventsTableModel *adminEventsModel = nullptr;
    EventsTableModel *userEventsModel = nullptr;
    QWidget *pieChartWidget = nullptr;
    QLineEdit *updateLineEdits[5];

    void mainLayout();

    void adminLayout();

    void adminAddEvent();

    void adminRemoveEvent();

    void setUpdateLineEdits();

    void adminUpdateEvent();

    void userRemoveEvent();

    void userOpenEvents();

    void userSelectMonth();

    QWidget* userListEvents(std::vector <Event*> eventsByMonth);

    bool isEventSelected(QTableView *table);

    static void clearLineEdits(QLineEdit *lineEdits[]);

    void centerWindow();

    void userLayout();

    QWidget* eventsPieChart();

    void changeWindowLayout(int index);

    void undo();

    void redo();

public:
    GUI(QApplication &app, Service &service);

    void start();

    static int exec();

    ~GUI();
};