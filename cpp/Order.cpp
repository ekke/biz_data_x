#include "Order.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString nrKey = "nr";
static const QString orderDateKey = "orderDate";
static const QString remarksKey = "remarks";
static const QString expressDeliveryKey = "expressDelivery";
static const QString positionsKey = "positions";
static const QString customerKey = "customer";

// keys used from Server API etc
static const QString nrForeignKey = "nr";
static const QString orderDateForeignKey = "orderDate";
static const QString remarksForeignKey = "remarks";
static const QString expressDeliveryForeignKey = "expressDelivery";
static const QString positionsForeignKey = "positions";
static const QString customerForeignKey = "customer";

/*
 * Default Constructor if Order not initialized from QVariantMap
 */
Order::Order(QObject *parent) :
        QObject(parent), mNr(-1), mRemarks(""), mExpressDelivery(false)
{
	// lazy references:
	mCustomer = -1;
	mCustomerAsDataObject = 0;
	mCustomerInvalid = false;
	// Date, Time or Timestamp ? construct null value
	mOrderDate = QDate();
}

bool Order::isAllResolved()
{
	if (hasCustomer() && !isCustomerResolvedAsDataObject()) {
		return false;
	}
    return true;
}

/*
 * initialize Order from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Order::fillFromMap(const QVariantMap& orderMap)
{
	mNr = orderMap.value(nrKey).toInt();
	if (orderMap.contains(orderDateKey)) {
		// always getting the Date as a String (from server or JSON)
		QString orderDateAsString = orderMap.value(orderDateKey).toString();
		mOrderDate = QDate::fromString(orderDateAsString, "yyyy-MM-dd");
		if (!mOrderDate.isValid()) {
			mOrderDate = QDate();
			qDebug() << "mOrderDate is not valid for String: " << orderDateAsString;
		}
	}
	mRemarks = orderMap.value(remarksKey).toString();
	mExpressDelivery = orderMap.value(expressDeliveryKey).toBool();
	// customer lazy pointing to Customer* (domainKey: nr)
	if (orderMap.contains(customerKey)) {
		mCustomer = orderMap.value(customerKey).toInt();
		if (mCustomer != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPositions is List of Position*
	QVariantList positionsList;
	positionsList = orderMap.value(positionsKey).toList();
	mPositions.clear();
	for (int i = 0; i < positionsList.size(); ++i) {
		QVariantMap positionsMap;
		positionsMap = positionsList.at(i).toMap();
		Position* position = new Position();
		position->setParent(this);
		position->fillFromMap(positionsMap);
		mPositions.append(position);
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Order::fillFromForeignMap(const QVariantMap& orderMap)
{
	mNr = orderMap.value(nrForeignKey).toInt();
	if (orderMap.contains(orderDateForeignKey)) {
		// always getting the Date as a String (from server or JSON)
		QString orderDateAsString = orderMap.value(orderDateForeignKey).toString();
		mOrderDate = QDate::fromString(orderDateAsString, "yyyy-MM-dd");
		if (!mOrderDate.isValid()) {
			mOrderDate = QDate();
			qDebug() << "mOrderDate is not valid for String: " << orderDateAsString;
		}
	}
	mRemarks = orderMap.value(remarksForeignKey).toString();
	mExpressDelivery = orderMap.value(expressDeliveryForeignKey).toBool();
	// customer lazy pointing to Customer* (domainKey: nr)
	if (orderMap.contains(customerForeignKey)) {
		mCustomer = orderMap.value(customerForeignKey).toInt();
		if (mCustomer != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPositions is List of Position*
	QVariantList positionsList;
	positionsList = orderMap.value(positionsForeignKey).toList();
	mPositions.clear();
	for (int i = 0; i < positionsList.size(); ++i) {
		QVariantMap positionsMap;
		positionsMap = positionsList.at(i).toMap();
		Position* position = new Position();
		position->setParent(this);
		position->fillFromForeignMap(positionsMap);
		mPositions.append(position);
	}
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Order::fillFromCacheMap(const QVariantMap& orderMap)
{
	mNr = orderMap.value(nrKey).toInt();
	if (orderMap.contains(orderDateKey)) {
		// always getting the Date as a String (from server or JSON)
		QString orderDateAsString = orderMap.value(orderDateKey).toString();
		mOrderDate = QDate::fromString(orderDateAsString, "yyyy-MM-dd");
		if (!mOrderDate.isValid()) {
			mOrderDate = QDate();
			qDebug() << "mOrderDate is not valid for String: " << orderDateAsString;
		}
	}
	mRemarks = orderMap.value(remarksKey).toString();
	mExpressDelivery = orderMap.value(expressDeliveryKey).toBool();
	// customer lazy pointing to Customer* (domainKey: nr)
	if (orderMap.contains(customerKey)) {
		mCustomer = orderMap.value(customerKey).toInt();
		if (mCustomer != -1) {
			// resolve the corresponding Data Object on demand from DataManager
		}
	}
	// mPositions is List of Position*
	QVariantList positionsList;
	positionsList = orderMap.value(positionsKey).toList();
	mPositions.clear();
	for (int i = 0; i < positionsList.size(); ++i) {
		QVariantMap positionsMap;
		positionsMap = positionsList.at(i).toMap();
		Position* position = new Position();
		position->setParent(this);
		position->fillFromCacheMap(positionsMap);
		mPositions.append(position);
	}
}

void Order::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Order::isValid()
{
	if (mNr == -1) {
		return false;
	}
	if (mPositions.size() == 0) {
		return false;
	}
	// customer lazy pointing to Customer* (domainKey: nr)
	if (mCustomer == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Order as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Order::toMap()
{
	QVariantMap orderMap;
	// customer lazy pointing to Customer* (domainKey: nr)
	if (mCustomer != -1) {
		orderMap.insert(customerKey, mCustomer);
	}
	orderMap.insert(nrKey, mNr);
	if (hasOrderDate()) {
		orderMap.insert(orderDateKey, mOrderDate.toString("yyyy-MM-dd"));
	}
	orderMap.insert(remarksKey, mRemarks);
	orderMap.insert(expressDeliveryKey, mExpressDelivery);
	// mPositions points to Position*
	orderMap.insert(positionsKey, positionsAsQVariantList());
	return orderMap;
}

/*
 * Exports Properties from Order as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Order::toForeignMap()
{
	QVariantMap orderMap;
	// customer lazy pointing to Customer* (domainKey: nr)
	if (mCustomer != -1) {
		orderMap.insert(customerForeignKey, mCustomer);
	}
	orderMap.insert(nrForeignKey, mNr);
	if (hasOrderDate()) {
		orderMap.insert(orderDateForeignKey, mOrderDate.toString("yyyy-MM-dd"));
	}
	orderMap.insert(remarksForeignKey, mRemarks);
	orderMap.insert(expressDeliveryForeignKey, mExpressDelivery);
	// mPositions points to Position*
	orderMap.insert(positionsForeignKey, positionsAsForeignQVariantList());
	return orderMap;
}


/*
 * Exports Properties from Order as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Order::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// REF
// Lazy: customer
// Mandatory: customer
// customer lazy pointing to Customer* (domainKey: nr)
int Order::customer() const
{
	return mCustomer;
}
Customer* Order::customerAsDataObject() const
{
	return mCustomerAsDataObject;
}
void Order::setCustomer(int customer)
{
	if (customer != mCustomer) {
        // remove old Data Object if one was resolved
        if (mCustomerAsDataObject) {
            // reset pointer, don't delete the independent object !
            mCustomerAsDataObject = 0;
        }
        // set the new lazy reference
        mCustomer = customer;
        mCustomerInvalid = false;
        emit customerChanged(customer);
        if (customer != -1) {
            // resolve the corresponding Data Object on demand from DataManager
        }
    }
}
void Order::removeCustomer()
{
	if (mCustomer != -1) {
		setCustomer(-1);
	}
}
bool Order::hasCustomer()
{
    if (!mCustomerInvalid && mCustomer != -1) {
        return true;
    } else {
        return false;
    }
}
bool Order::isCustomerResolvedAsDataObject()
{
    if (!mCustomerInvalid && mCustomerAsDataObject) {
        return true;
    } else {
        return false;
    }
}

// lazy bound Data Object was resolved. overwrite nr if different
void Order::resolveCustomerAsDataObject(Customer* customer)
{
    if (customer) {
        if (customer->nr() != mCustomer) {
            setCustomer(customer->nr());
        }
        mCustomerAsDataObject = customer;
        mCustomerInvalid = false;
    }
}
void Order::markCustomerAsInvalid()
{
    mCustomerInvalid = true;
}
// ATT 
// Mandatory: nr
// Domain KEY: nr
int Order::nr() const
{
	return mNr;
}

void Order::setNr(int nr)
{
	if (nr != mNr) {
		mNr = nr;
		emit nrChanged(nr);
	}
}
// ATT 
// Optional: orderDate
/**
 * in QML set DateTimePicker value this way:
 * myPicker.value = new Date(orderDate)
 */
QDate Order::orderDate() const
{
	return mOrderDate;
}

/**
 * from QML DateTime Picker use as parameter:
 * orderDate = new Date(myPicker.value)
 */
void Order::setOrderDate(QDate orderDate)
{
	if (orderDate != mOrderDate) {
		mOrderDate = orderDate;
		emit orderDateChanged(orderDate);
	}
}
bool Order::hasOrderDate()
{
	return !mOrderDate.isNull() && mOrderDate.isValid();
}
// ATT 
// Optional: remarks
QString Order::remarks() const
{
	return mRemarks;
}

void Order::setRemarks(QString remarks)
{
	if (remarks != mRemarks) {
		mRemarks = remarks;
		emit remarksChanged(remarks);
	}
}
// ATT 
// Optional: expressDelivery
bool Order::expressDelivery() const
{
	return mExpressDelivery;
}

void Order::setExpressDelivery(bool expressDelivery)
{
	if (expressDelivery != mExpressDelivery) {
		mExpressDelivery = expressDelivery;
		emit expressDeliveryChanged(expressDelivery);
	}
}
// REF
// Opposite: orderHeader
// Mandatory: positions
QVariantList Order::positionsAsQVariantList()
{
	QVariantList positionsList;
	for (int i = 0; i < mPositions.size(); ++i) {
        positionsList.append((mPositions.at(i))->toMap());
    }
	return positionsList;
}
QVariantList Order::positionsAsForeignQVariantList()
{
	QVariantList positionsList;
	for (int i = 0; i < mPositions.size(); ++i) {
        positionsList.append((mPositions.at(i))->toForeignMap());
    }
	return positionsList;
}
/**
 * creates a new Position
 * parent is this Order
 * if data is successfully entered you must INVOKE addToPositions()
 * if edit was canceled you must undoCreateElementOfPositions to free up memory
 */
Position* Order::createElementOfPositions()
{
    Position* position;
    position = new Position();
    position->setParent(this);
    position->prepareNew();
    return position;
}

/**
 * if createElementOfPositions was canceled from UI
 * this method deletes the Object of Type Position
 * 
 * to delete a allready into positions inserted  Position
 * you must use removeFromPositions
 */
void Order::undoCreateElementOfPositions(Position* position)
{
    if (position) {
        position->deleteLater();
        position = 0;
    }
}
/**
 * you can add positions without resolving existing keys before
 * attention: before looping through the objects
 * you must resolvePositionsKeys
 */
void Order::addToPositions(Position* position)
{
    mPositions.append(position);
    emit addedToPositions(position);
    emit positionsPropertyListChanged();
}

bool Order::removeFromPositions(Position* position)
{
    bool ok = false;
    ok = mPositions.removeOne(position);
    if (!ok) {
    	qDebug() << "Position* not found in positions";
    	return false;
    }
    emit removedFromPositionsByUuid(position->uuid());
    emit positionsPropertyListChanged();
    // positions are contained - so we must delete them
    position->deleteLater();
    position = 0;
    return true;
}
void Order::clearPositions()
{
    for (int i = mPositions.size(); i > 0; --i) {
        removeFromPositions(mPositions.last());
    }
}
void Order::addToPositionsFromMap(const QVariantMap& positionMap)
{
    Position* position = new Position();
    position->setParent(this);
    position->fillFromMap(positionMap);
    mPositions.append(position);
    emit addedToPositions(position);
    emit positionsPropertyListChanged();
}
bool Order::removeFromPositionsByUuid(const QString& uuid)
{
    for (int i = 0; i < mPositions.size(); ++i) {
    	Position* position;
        position = mPositions.at(i);
        if (position->uuid() == uuid) {
        	mPositions.removeAt(i);
        	emit removedFromPositionsByUuid(uuid);
        	emit positionsPropertyListChanged();
        	// positions are contained - so we must delete them
        	position->deleteLater();
        	position = 0;
        	return true;
        }
    }
    qDebug() << "uuid not found in positions: " << uuid;
    return false;
}

int Order::positionsCount()
{
    return mPositions.size();
}
QList<Position*> Order::positions()
{
	return mPositions;
}
void Order::setPositions(QList<Position*> positions) 
{
	if (positions != mPositions) {
		mPositions = positions;
		emit positionsChanged(positions);
	}
}

/**
 * to access lists from QML we're using QQmlListProperty
 * and implement methods to append, count and clear
 * now from QML we can use
 * order.positionsPropertyList.length to get the size
 * order.positionsPropertyList[2] to get Position* at position 2
 * order.positionsPropertyList = [] to clear the list
 * or get easy access to properties like
 * order.positionsPropertyList[2].myPropertyName
 */
QQmlListProperty<Position> Order::positionsPropertyList()
{
    return QQmlListProperty<Position>(this, 0, &Order::appendToPositionsProperty,
            &Order::positionsPropertyCount, &Order::atPositionsProperty,
            &Order::clearPositionsProperty);
}
void Order::appendToPositionsProperty(QQmlListProperty<Position> *positionsList,
        Position* position)
{
    Order *orderObject = qobject_cast<Order *>(positionsList->object);
    if (orderObject) {
		position->setParent(orderObject);
        orderObject->mPositions.append(position);
        emit orderObject->addedToPositions(position);
    } else {
        qWarning() << "cannot append Position* to positions " << "Object is not of type Order*";
    }
}
int Order::positionsPropertyCount(QQmlListProperty<Position> *positionsList)
{
    Order *order = qobject_cast<Order *>(positionsList->object);
    if (order) {
        return order->mPositions.size();
    } else {
        qWarning() << "cannot get size positions " << "Object is not of type Order*";
    }
    return 0;
}
Position* Order::atPositionsProperty(QQmlListProperty<Position> *positionsList, int pos)
{
    Order *order = qobject_cast<Order *>(positionsList->object);
    if (order) {
        if (order->mPositions.size() > pos) {
            return order->mPositions.at(pos);
        }
        qWarning() << "cannot get Position* at pos " << pos << " size is "
                << order->mPositions.size();
    } else {
        qWarning() << "cannot get Position* at pos " << pos << "Object is not of type Order*";
    }
    return 0;
}
void Order::clearPositionsProperty(QQmlListProperty<Position> *positionsList)
{
    Order *order = qobject_cast<Order *>(positionsList->object);
    if (order) {
        // positions are contained - so we must delete them
        for (int i = 0; i < order->mPositions.size(); ++i) {
            order->mPositions.at(i)->deleteLater();
        }
        order->mPositions.clear();
    } else {
        qWarning() << "cannot clear positions " << "Object is not of type Order*";
    }
}



Order::~Order()
{
	// place cleanUp code here
}
	
