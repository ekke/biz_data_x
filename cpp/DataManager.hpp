#ifndef DATAMANAGER_HPP_
#define DATAMANAGER_HPP_

#include <qobject.h>
#include <QQmlListProperty>
#include <QStringList>

#include "Customer.hpp"
#include "Order.hpp"
#include "Position.hpp"
#include "SettingsData.hpp"

class DataManager: public QObject
{
Q_OBJECT

// QDeclarativeListProperty to get easy access from QML
Q_PROPERTY(QQmlListProperty<Customer> customerPropertyList READ customerPropertyList CONSTANT)
Q_PROPERTY(QQmlListProperty<Order> orderPropertyList READ orderPropertyList CONSTANT)

public:
    DataManager(QObject *parent = 0);
    virtual ~DataManager();
    Q_INVOKABLE
    void init();

    bool checkDirs();

	
//	Q_INVOKABLE
//	void fillCustomerDataModel(QString objectName);

//	Q_INVOKABLE
//	void replaceItemInCustomerDataModel(QString objectName, Customer* listItem);

//	Q_INVOKABLE
//	void removeItemFromCustomerDataModel(QString objectName, Customer* listItem);

//	Q_INVOKABLE
//	void insertItemIntoCustomerDataModel(QString objectName, Customer* listItem);

	Q_INVOKABLE
	QList<Customer*> listOfCustomerForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList customerAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allCustomer();

	Q_INVOKABLE
	void deleteCustomer();

	// access from QML to list of all Customer
    QQmlListProperty<Customer> customerPropertyList();

	Q_INVOKABLE
	Customer* createCustomer();

	Q_INVOKABLE
	void undoCreateCustomer(Customer* customer);

	Q_INVOKABLE
	void insertCustomer(Customer* customer);

	Q_INVOKABLE
	void insertCustomerFromMap(const QVariantMap& customerMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteCustomer(Customer* customer);

	Q_INVOKABLE
	bool deleteCustomerByNr(const int& nr);

	Q_INVOKABLE
    Customer* findCustomerByNr(const int& nr);
	
//	Q_INVOKABLE
//	void fillOrderDataModel(QString objectName);

//	Q_INVOKABLE
//	void replaceItemInOrderDataModel(QString objectName, Order* listItem);

//	Q_INVOKABLE
//	void removeItemFromOrderDataModel(QString objectName, Order* listItem);

//	Q_INVOKABLE
//	void insertItemIntoOrderDataModel(QString objectName, Order* listItem);

	Q_INVOKABLE
	void resolveOrderReferences(Order* order);

	Q_INVOKABLE
	void resolveReferencesForAllOrder();

	Q_INVOKABLE
	QList<Order*> listOfOrderForKeys(QStringList keyList);

	Q_INVOKABLE
	QVariantList orderAsQVariantList();

	Q_INVOKABLE
	QList<QObject*> allOrder();

	Q_INVOKABLE
	void deleteOrder();

	// access from QML to list of all Order
    QQmlListProperty<Order> orderPropertyList();

	Q_INVOKABLE
	Order* createOrder();

	Q_INVOKABLE
	void undoCreateOrder(Order* order);

	Q_INVOKABLE
	void insertOrder(Order* order);

	Q_INVOKABLE
	void insertOrderFromMap(const QVariantMap& orderMap, const bool& useForeignProperties);

	Q_INVOKABLE
	bool deleteOrder(Order* order);

	Q_INVOKABLE
	bool deleteOrderByNr(const int& nr);

	Q_INVOKABLE
    Order* findOrderByNr(const int& nr);

	Q_INVOKABLE
    SettingsData* settingsData();

    void initCustomerFromCache();
    void initOrderFromCache();
    void initSettingsDataFromCache();
    void finish();

Q_SIGNALS:

	void shuttingDown();
	void addedToAllCustomer(Customer* customer);
	void deletedFromAllCustomerByNr(int nr);
	void deletedFromAllCustomer(Customer* customer);
	void addedToAllOrder(Order* order);
	void deletedFromAllOrderByNr(int nr);
	void deletedFromAllOrder(Order* order);
    
public slots:

private:

    QString mDataRoot;
    QString mDataPath;
    QString mDataAssetsPath;
    QString dataAssetsPath(const QString& fileName);
    QString dataPath(const QString& fileName);

    SettingsData* mSettingsData;
    void readSettings();
    void saveSettings();

	// DataObject stored in List of QObject*
	// GroupDataModel only supports QObject*
    QList<QObject*> mAllCustomer;
    // implementation for QQmlListProperty to use
    // QML functions for List of All Customer*
    static void appendToCustomerProperty(
        QQmlListProperty<Customer> *customerList,
    	Customer* customer);
    static int customerPropertyCount(
        QQmlListProperty<Customer> *customerList);
    static Customer* atCustomerProperty(
        QQmlListProperty<Customer> *customerList, int pos);
    static void clearCustomerProperty(
        QQmlListProperty<Customer> *customerList);
    QList<QObject*> mAllOrder;
    // implementation for QDeclarativeListProperty to use
    // QML functions for List of All Order*
    static void appendToOrderProperty(
        QQmlListProperty<Order> *orderList,
    	Order* order);
    static int orderPropertyCount(
        QQmlListProperty<Order> *orderList);
    static Order* atOrderProperty(
        QQmlListProperty<Order> *orderList, int pos);
    static void clearOrderProperty(
        QQmlListProperty<Order> *orderList);

    void saveCustomerToCache();
    void saveOrderToCache();


	QVariantList readFromCache(const QString& fileName);
	void writeToCache(const QString& fileName, QVariantList& data);
};

#endif /* DATAMANAGER_HPP_ */
