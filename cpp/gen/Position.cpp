#include "Position.hpp"
#include <QDebug>
#include <quuid.h>
#include "Order.hpp"

// keys of QVariantMap used in this APP
static const QString uuidKey = "uuid";
static const QString descriptionKey = "description";
static const QString priceKey = "price";
// no key for orderHeader

// keys used from Server API etc
static const QString uuidForeignKey = "uuid";
static const QString descriptionForeignKey = "description";
static const QString priceForeignKey = "price";
// no key for orderHeader

/*
 * Default Constructor if Position not initialized from QVariantMap
 */
Position::Position(QObject *parent) :
        QObject(parent), mUuid(""), mDescription(""), mPrice(0.0)
{
}

/*
 * initialize Position from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Position::fillFromMap(const QVariantMap& positionMap)
{
	mUuid = positionMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mDescription = positionMap.value(descriptionKey).toString();
	mPrice = positionMap.value(priceKey).toDouble();
	// mOrderHeader is parent (Order* containing Position)
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Position::fillFromForeignMap(const QVariantMap& positionMap)
{
	mUuid = positionMap.value(uuidForeignKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mDescription = positionMap.value(descriptionForeignKey).toString();
	mPrice = positionMap.value(priceForeignKey).toDouble();
	// mOrderHeader is parent (Order* containing Position)
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Position::fillFromCacheMap(const QVariantMap& positionMap)
{
	mUuid = positionMap.value(uuidKey).toString();
	if (mUuid.isEmpty()) {
		mUuid = QUuid::createUuid().toString();
		mUuid = mUuid.right(mUuid.length() - 1);
		mUuid = mUuid.left(mUuid.length() - 1);
	}	
	mDescription = positionMap.value(descriptionKey).toString();
	mPrice = positionMap.value(priceKey).toDouble();
	// mOrderHeader is parent (Order* containing Position)
}

void Position::prepareNew()
{
	mUuid = QUuid::createUuid().toString();
	mUuid = mUuid.right(mUuid.length() - 1);
	mUuid = mUuid.left(mUuid.length() - 1);
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Position::isValid()
{
	if (mUuid.isNull() || mUuid.isEmpty()) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Position as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Position::toMap()
{
	QVariantMap positionMap;
	positionMap.insert(uuidKey, mUuid);
	positionMap.insert(descriptionKey, mDescription);
	positionMap.insert(priceKey, mPrice);
	// mOrderHeader points to Order* containing Position
	return positionMap;
}

/*
 * Exports Properties from Position as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Position::toForeignMap()
{
	QVariantMap positionMap;
	positionMap.insert(uuidForeignKey, mUuid);
	positionMap.insert(descriptionForeignKey, mDescription);
	positionMap.insert(priceForeignKey, mPrice);
	// mOrderHeader points to Order* containing Position
	return positionMap;
}


/*
 * Exports Properties from Position as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Position::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: uuid
// Domain KEY: uuid
QString Position::uuid() const
{
	return mUuid;
}

void Position::setUuid(QString uuid)
{
	if (uuid != mUuid) {
		mUuid = uuid;
		emit uuidChanged(uuid);
	}
}
// ATT 
// Optional: description
QString Position::description() const
{
	return mDescription;
}

void Position::setDescription(QString description)
{
	if (description != mDescription) {
		mDescription = description;
		emit descriptionChanged(description);
	}
}
// ATT 
// Optional: price
double Position::price() const
{
	return mPrice;
}

void Position::setPrice(double price)
{
	if (price != mPrice) {
		mPrice = price;
		emit priceChanged(price);
	}
}
// REF
// Opposite: positions
// Optional: orderHeader
// No SETTER for OrderHeader - it's the parent
Order* Position::orderHeader() const
{
	return qobject_cast<Order*>(parent());
}


Position::~Position()
{
	// place cleanUp code here
}
	
