#ifndef CUSTOMER_HPP_
#define CUSTOMER_HPP_

#include <QObject>
#include <qvariant.h>




class Customer: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int nr READ nr WRITE setNr NOTIFY nrChanged FINAL)
	Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
	Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged FINAL)
	Q_PROPERTY(QString zip READ zip WRITE setZip NOTIFY zipChanged FINAL)
	Q_PROPERTY(QString street READ street WRITE setStreet NOTIFY streetChanged FINAL)
	Q_PROPERTY(bool hasAddress READ hasAddress WRITE setHasAddress NOTIFY hasAddressChanged FINAL)
	Q_PROPERTY(bool hasOpenOrders READ hasOpenOrders WRITE setHasOpenOrders NOTIFY hasOpenOrdersChanged FINAL)
	Q_PROPERTY(int abc READ abc WRITE setAbc NOTIFY abcChanged FINAL)


public:
	Customer(QObject *parent = 0);


	void fillFromMap(const QVariantMap& customerMap);
	void fillFromForeignMap(const QVariantMap& customerMap);
	void fillFromCacheMap(const QVariantMap& customerMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int nr() const;
	void setNr(int nr);
	QString name() const;
	void setName(QString name);
	QString city() const;
	void setCity(QString city);
	QString zip() const;
	void setZip(QString zip);
	QString street() const;
	void setStreet(QString street);
	bool hasAddress() const;
	void setHasAddress(bool hasAddress);
	bool hasOpenOrders() const;
	void setHasOpenOrders(bool hasOpenOrders);
	int abc() const;
	void setAbc(int abc);



	virtual ~Customer();

	Q_SIGNALS:

	void nrChanged(int nr);
	void nameChanged(QString name);
	void cityChanged(QString city);
	void zipChanged(QString zip);
	void streetChanged(QString street);
	void hasAddressChanged(bool hasAddress);
	void hasOpenOrdersChanged(bool hasOpenOrders);
	void abcChanged(int abc);
	

private:

	int mNr;
	QString mName;
	QString mCity;
	QString mZip;
	QString mStreet;
	bool mHasAddress;
	bool mHasOpenOrders;
	int mAbc;

	Q_DISABLE_COPY (Customer)
};
Q_DECLARE_METATYPE(Customer*)

#endif /* CUSTOMER_HPP_ */

