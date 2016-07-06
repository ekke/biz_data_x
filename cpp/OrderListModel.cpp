#include "OrderListModel.hpp"
#include <qdebug>

OrderListModel::OrderListModel(QObject *parent) : QAbstractListModel(parent)
{
	//
}

int OrderListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_objects.count();
}

QVariant OrderListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::UserRole)
        return QVariant();

    return QVariant::fromValue(m_objects.at(index.row()));
}

QHash<int, QByteArray> OrderListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::UserRole, "example");
    return roles;
}

void OrderListModel::appendObject()
{
    emit beginInsertRows(QModelIndex(), m_objects.size(), m_objects.size());
    Kunde* kunde = new Kunde(this);
    kunde->setName("WOW");
    m_objects.append(kunde);
    emit endInsertRows();

    qDebug() << "COUNT " << m_objects.count();

    ((Kunde*)m_objects.at(3))->setName("4711");

    emit beginRemoveRows(QModelIndex(), 7, 7);
    m_objects.removeAt(7);
    emit endRemoveRows();
}
