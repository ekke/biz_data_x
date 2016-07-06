#ifndef ORDERLISTMODEL_HPP
#define ORDERLISTMODEL_HPP

#include <QObject>
#include <QAbstractListModel>
#include "DataManager.hpp"

class OrderListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit OrderListModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    void appendObject();

signals:

public slots:

private:
    // QObjectList m_objects;

};

#endif // ORDERLISTMODEL_HPP
