#include "Customer.hpp"
#include <QDebug>
#include <quuid.h>

// keys of QVariantMap used in this APP
static const QString nrKey = "nr";
static const QString nameKey = "name";
static const QString cityKey = "city";
static const QString zipKey = "zip";
static const QString streetKey = "street";
static const QString hasAddressKey = "hasAddress";
static const QString hasOpenOrdersKey = "hasOpenOrders";
static const QString abcKey = "abc";

// keys used from Server API etc
static const QString nrForeignKey = "nr";
static const QString nameForeignKey = "name";
static const QString cityForeignKey = "city";
static const QString zipForeignKey = "zip";
static const QString streetForeignKey = "street";
static const QString hasAddressForeignKey = "hasAddress";
static const QString hasOpenOrdersForeignKey = "hasOpenOrders";
static const QString abcForeignKey = "abc";

/*
 * Default Constructor if Customer not initialized from QVariantMap
 */
Customer::Customer(QObject *parent) :
        QObject(parent), mNr(-1), mName(""), mCity(""), mZip(""), mStreet(""), mHasAddress(false), mHasOpenOrders(false), mAbc(0)
{
}

/*
 * initialize Customer from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses own property names
 * corresponding export method: toMap()
 */
void Customer::fillFromMap(const QVariantMap& customerMap)
{
	mNr = customerMap.value(nrKey).toInt();
	mName = customerMap.value(nameKey).toString();
	mCity = customerMap.value(cityKey).toString();
	mZip = customerMap.value(zipKey).toString();
	mStreet = customerMap.value(streetKey).toString();
	mHasAddress = customerMap.value(hasAddressKey).toBool();
	mHasOpenOrders = customerMap.value(hasOpenOrdersKey).toBool();
	mAbc = customerMap.value(abcKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * includes also transient values
 * uses foreign property names - per ex. from Server API
 * corresponding export method: toForeignMap()
 */
void Customer::fillFromForeignMap(const QVariantMap& customerMap)
{
	mNr = customerMap.value(nrForeignKey).toInt();
	mName = customerMap.value(nameForeignKey).toString();
	mCity = customerMap.value(cityForeignKey).toString();
	mZip = customerMap.value(zipForeignKey).toString();
	mStreet = customerMap.value(streetForeignKey).toString();
	mHasAddress = customerMap.value(hasAddressForeignKey).toBool();
	mHasOpenOrders = customerMap.value(hasOpenOrdersForeignKey).toBool();
	mAbc = customerMap.value(abcForeignKey).toInt();
}
/*
 * initialize OrderData from QVariantMap
 * Map got from JsonDataAccess or so
 * excludes transient values
 * uses own property names
 * corresponding export method: toCacheMap()
 */
void Customer::fillFromCacheMap(const QVariantMap& customerMap)
{
	mNr = customerMap.value(nrKey).toInt();
	mName = customerMap.value(nameKey).toString();
	mCity = customerMap.value(cityKey).toString();
	mZip = customerMap.value(zipKey).toString();
	mStreet = customerMap.value(streetKey).toString();
	mHasAddress = customerMap.value(hasAddressKey).toBool();
	mHasOpenOrders = customerMap.value(hasOpenOrdersKey).toBool();
	mAbc = customerMap.value(abcKey).toInt();
}

void Customer::prepareNew()
{
}

/*
 * Checks if all mandatory attributes, all DomainKeys and uuid's are filled
 */
bool Customer::isValid()
{
	if (mNr == -1) {
		return false;
	}
	return true;
}
	
/*
 * Exports Properties from Customer as QVariantMap
 * exports ALL data including transient properties
 * To store persistent Data in JsonDataAccess use toCacheMap()
 */
QVariantMap Customer::toMap()
{
	QVariantMap customerMap;
	customerMap.insert(nrKey, mNr);
	customerMap.insert(nameKey, mName);
	customerMap.insert(cityKey, mCity);
	customerMap.insert(zipKey, mZip);
	customerMap.insert(streetKey, mStreet);
	customerMap.insert(hasAddressKey, mHasAddress);
	customerMap.insert(hasOpenOrdersKey, mHasOpenOrders);
	customerMap.insert(abcKey, mAbc);
	return customerMap;
}

/*
 * Exports Properties from Customer as QVariantMap
 * To send data as payload to Server
 * Makes it possible to use defferent naming conditions
 */
QVariantMap Customer::toForeignMap()
{
	QVariantMap customerMap;
	customerMap.insert(nrForeignKey, mNr);
	customerMap.insert(nameForeignKey, mName);
	customerMap.insert(cityForeignKey, mCity);
	customerMap.insert(zipForeignKey, mZip);
	customerMap.insert(streetForeignKey, mStreet);
	customerMap.insert(hasAddressForeignKey, mHasAddress);
	customerMap.insert(hasOpenOrdersForeignKey, mHasOpenOrders);
	customerMap.insert(abcForeignKey, mAbc);
	return customerMap;
}


/*
 * Exports Properties from Customer as QVariantMap
 * transient properties are excluded:
 * To export ALL data use toMap()
 */
QVariantMap Customer::toCacheMap()
{
	// no transient properties found from data model
	// use default toMao()
	return toMap();
}
// ATT 
// Mandatory: nr
// Domain KEY: nr
int Customer::nr() const
{
	return mNr;
}

void Customer::setNr(int nr)
{
	if (nr != mNr) {
		mNr = nr;
		emit nrChanged(nr);
	}
}
// ATT 
// Optional: name
QString Customer::name() const
{
	return mName;
}

void Customer::setName(QString name)
{
	if (name != mName) {
		mName = name;
		emit nameChanged(name);
	}
}
// ATT 
// Optional: city
QString Customer::city() const
{
	return mCity;
}

void Customer::setCity(QString city)
{
	if (city != mCity) {
		mCity = city;
		emit cityChanged(city);
	}
}
// ATT 
// Optional: zip
QString Customer::zip() const
{
	return mZip;
}

void Customer::setZip(QString zip)
{
	if (zip != mZip) {
		mZip = zip;
		emit zipChanged(zip);
	}
}
// ATT 
// Optional: street
QString Customer::street() const
{
	return mStreet;
}

void Customer::setStreet(QString street)
{
	if (street != mStreet) {
		mStreet = street;
		emit streetChanged(street);
	}
}
// ATT 
// Optional: hasAddress
bool Customer::hasAddress() const
{
	return mHasAddress;
}

void Customer::setHasAddress(bool hasAddress)
{
	if (hasAddress != mHasAddress) {
		mHasAddress = hasAddress;
		emit hasAddressChanged(hasAddress);
	}
}
// ATT 
// Optional: hasOpenOrders
bool Customer::hasOpenOrders() const
{
	return mHasOpenOrders;
}

void Customer::setHasOpenOrders(bool hasOpenOrders)
{
	if (hasOpenOrders != mHasOpenOrders) {
		mHasOpenOrders = hasOpenOrders;
		emit hasOpenOrdersChanged(hasOpenOrders);
	}
}
// ATT 
// Optional: abc
int Customer::abc() const
{
	return mAbc;
}

void Customer::setAbc(int abc)
{
	if (abc != mAbc) {
		mAbc = abc;
		emit abcChanged(abc);
	}
}


Customer::~Customer()
{
	// place cleanUp code here
}
	
