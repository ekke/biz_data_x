#ifndef POSITION_HPP_
#define POSITION_HPP_

#include <QObject>
#include <qvariant.h>


// forward declaration to avoid circular dependencies
class Order;


class Position: public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString uuid READ uuid WRITE setUuid NOTIFY uuidChanged FINAL)
	Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
	Q_PROPERTY(double price READ price WRITE setPrice NOTIFY priceChanged FINAL)
	Q_PROPERTY(Order* orderHeader READ orderHeader)


public:
	Position(QObject *parent = 0);


	void fillFromMap(const QVariantMap& positionMap);
	void fillFromForeignMap(const QVariantMap& positionMap);
	void fillFromCacheMap(const QVariantMap& positionMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	QString uuid() const;
	void setUuid(QString uuid);
	QString description() const;
	void setDescription(QString description);
	double price() const;
	void setPrice(double price);
	Order* orderHeader() const;
	// no SETTER orderHeader() is only convenience method to get the parent



	virtual ~Position();

	Q_SIGNALS:

	void uuidChanged(QString uuid);
	void descriptionChanged(QString description);
	void priceChanged(double price);
	// no SIGNAL orderHeader is only convenience way to get the parent
	

private:

	QString mUuid;
	QString mDescription;
	double mPrice;
	// no MEMBER mOrderHeader it's the parent

	Q_DISABLE_COPY (Position)
};
Q_DECLARE_METATYPE(Position*)

#endif /* POSITION_HPP_ */

