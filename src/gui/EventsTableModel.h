#pragma once

#include <QAbstractTableModel>
#include "../domain/Event.h"
#include <QStyledItemDelegate>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>


class LinkDelegate : public QStyledItemDelegate {
public:
    LinkDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    QRect rect = opt.widget->style()->subElementRect(QStyle::SE_ItemViewItemText, &opt);

    QString text = index.data().toString();
    QFontMetrics fm(opt.font);
    QRect boundingRect = fm.boundingRect(rect, Qt::AlignLeft | Qt::AlignVCenter, text);

    QRect linkRect(boundingRect.left(), boundingRect.top(), boundingRect.width(), boundingRect.height());
    painter->setPen(Qt::blue);
    painter->drawText(linkRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override {
        if (event->type() == QEvent::MouseButtonRelease) {
            auto *mouseEvent = dynamic_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                QString link = index.data().toString();
                QDesktopServices::openUrl(QUrl(link));
                return true;
            }
        }
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
};

class EventsTableModel : public QAbstractTableModel {
private:
    std::vector<Event> events;
public:
    explicit EventsTableModel(std::vector<Event> events, QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void setEvents(std::vector<Event> events);

    void updateEvents(std::vector<Event> newEvents);
};
