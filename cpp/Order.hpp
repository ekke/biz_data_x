#ifndef ORDER_HPP_
#define ORDER_HPP_

#include <QObject>
#include <qvariant.h>
#include <QQmlListProperty>
#include <QDate>


#include "Position.hpp"
#include "Customer.hpp"


class Order: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int nr READ nr WRITE setNr NOTIFY nrChanged FINAL)
	Q_PROPERTY(QDate orderDate READ orderDate WRITE setOrderDate NOTIFY orderDateChanged FINAL)
	Q_PROPERTY(QString remarks READ remarks WRITE setRemarks NOTIFY remarksChanged FINAL)
	Q_PROPERTY(bool expressDelivery READ expressDelivery WRITE setExpressDelivery NOTIFY expressDeliveryChanged FINAL)
	// customer lazy pointing to Customer* (domainKey: nr)
	Q_PROPERTY(int customer READ customer WRITE setCustomer NOTIFY customerChanged FINAL)
	Q_PROPERTY(Customer* customerAsDataObject READ customerAsDataObject WRITE resolveCustomerAsDataObject NOTIFY customerAsDataObjectChanged FINAL)

	// QDeclarativeListProperty to get easy access from QML
    Q_PROPERTY(QQmlListProperty<Position> positionsPropertyList READ positionsPropertyList CONSTANT)

public:
	Order(QObject *parent = 0);

	Q_INVOKABLE
	bool isAllResolved();

	void fillFromMap(const QVariantMap& orderMap);
	void fillFromForeignMap(const QVariantMap& orderMap);
	void fillFromCacheMap(const QVariantMap& orderMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int nr() const;
	void setNr(int nr);
	QDate orderDate() const;

	Q_INVOKABLE
	bool hasOrderDate();
	void setOrderDate(QDate orderDate);
	QString remarks() const;
	void setRemarks(QString remarks);
	bool expressDelivery() const;
	void setExpressDelivery(bool expressDelivery);
	// customer lazy pointing to Customer* (domainKey: nr)
	int customer() const;
	void setCustomer(int customer);
	Customer* customerAsDataObject() const;
	
	Q_INVOKABLE
	void resolveCustomerAsDataObject(Customer* customer);
	
	Q_INVOKABLE
	void removeCustomer();
	
	Q_INVOKABLE
	bool hasCustomer();
	
	Q_INVOKABLE
	bool isCustomerResolvedAsDataObject();
	
	Q_INVOKABLE
	void markCustomerAsInvalid();
	

	
	Q_INVOKABLE
	QVariantList positionsAsQVariantList();
	
	Q_INVOKABLE
	QVariantList positionsAsForeignQVariantList();

	Q_INVOKABLE
	Position* createElementOfPositions();

	Q_INVOKABLE
	void undoCreateElementOfPositions(Position* position);
	
	Q_INVOKABLE
	void addToPositions(Position* position);
	
	Q_INVOKABLE
	bool removeFromPositions(Position* position);

	Q_INVOKABLE
	void clearPositions();

	Q_INVOKABLE
	void addToPositionsFromMap(const QVariantMap& positionMap);
	
	Q_INVOKABLE
	bool removeFromPositionsByUuid(const QString& uuid);
	
	Q_INVOKABLE
	int positionsCount();
	
	 // access from C++ to positions
	QList<Position*> positions();
	void setPositions(QList<Position*> positions);
	// access from QML to positions
    QQmlListProperty<Position> positionsPropertyList();


	virtual ~Order();

	Q_SIGNALS:

	void nrChanged(int nr);
	void orderDateChanged(QDate orderDate);
	void remarksChanged(QString remarks);
	void expressDeliveryChanged(bool expressDelivery);
	// customer lazy pointing to Customer* (domainKey: nr)
	void customerChanged(int customer);
	void customerAsDataObjectChanged(Customer* customer);
	void positionsChanged(QList<Position*> positions);
	void addedToPositions(Position* position);
	void removedFromPositionsByUuid(QString uuid);
	
	

private:

	int mNr;
	QDate mOrderDate;
	QString mRemarks;
	bool mExpressDelivery;
	int mCustomer;
	bool mCustomerInvalid;
	Customer* mCustomerAsDataObject;
	QList<Position*> mPositions;
	// implementation for QDeclarativeListProperty to use
	// QML functions for List of Position*
    static void appendToPositionsProperty(QQmlListProperty<Position> *positionsList,
		Position* position);
    static int positionsPropertyCount(QQmlListProperty<Position> *positionsList);
    static Position* atPositionsProperty(QQmlListProperty<Position> *positionsList, int pos);
    static void clearPositionsProperty(QQmlListProperty<Position> *positionsList);

	Q_DISABLE_COPY (Order)
};
Q_DECLARE_METATYPE(Order*)

#endif /* ORDER_HPP_ */

