#include <QObject>

#include "DataManager.hpp"

#include <QtQml>
#include <QJsonObject>
#include <QFile>


static const QString PRODUCTION_ENVIRONMENT = "prod/";
static const QString TEST_ENVIRONMENT = "test/";
static bool isProductionEnvironment = true;


static const QString cacheCustomer = "cacheCustomer.json";
static const QString cacheOrder = "cacheOrder.json";
static const QString cacheSettingsData = "cacheSettingsData.json";


DataManager::DataManager(QObject *parent) :
        QObject(parent)
{
   // Android: HomeLocation works, iOS: not writable
    // Android: AppDataLocation works out of the box, iOS you must create the DIR first !!
    mDataRoot = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);
    mDataPath = mDataRoot+"/data/";
    mDataAssetsPath = ":/data-assets/";
    qDebug() << "Data Path: " << mDataPath << " data-assets: " << mDataAssetsPath;
    // guarantee that dirs exist
    bool ok = checkDirs();
    if(!ok) {
        qFatal("App won't work - cannot create data directory");
    }

    // at first read settingsData (always from Sandbox)
    mSettingsPath = mDataRoot+"/"+cacheSettingsData;
    qDebug() << "Settings Path: " << mSettingsPath;
    readSettings();

#ifdef QT_DEBUG
    qDebug() << "Running a DEBUG BUILD";
    // DEBUG MODE ?
    // now check if public cache is used
    if (mSettingsData->hasPublicCache()) {
        // great while testing: access files from file explorer
        // only works on Android - on iOS it helps to use a 2nd cache for dev
        mDataRoot = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation).value(0);
        // per ex. /data/ekkescorner/theAppName
        mDataRoot += mSettingsData->publicRoot4Dev();
        mDataPath = mDataRoot+"/data/";
        ok = checkDirs();
        if(!ok) {
            qFatal("App won't work - cannot create data directory");
        }
        qDebug() << "Data Path redirected to PUBLIC CACHE: " << mDataPath;
        // tip: copy settingsData to public cache to see the content
        // but settings will always be used from AppDataLocation
    }
#else
    qDebug() << "Running a RELEASE BUILD";
    // always use compact JSON in release builds
    mSettingsData->setUseCompactJsonFormat(true);
    // never use public data path in releae build
    mSettingsData->setPublicRoot4Dev(false);
#endif
    // now set the compact or indent mode for JSON Documents
    mCompactJson = mSettingsData->useCompactJsonFormat();
	isProductionEnvironment = mSettingsData->isProductionEnvironment();

    // ApplicationUI is parent of DataManager
    // DataManager is parent of all root DataObjects
    // ROOT DataObjects are parent of contained DataObjects
    // ROOT:
    // Customer
    // Order
    // SettingsData

    // register all DataObjects to get access to properties from QML:
	qmlRegisterType<Customer>("org.ekkescorner.data", 1, 0, "Customer");
	qmlRegisterType<Order>("org.ekkescorner.data", 1, 0, "Order");
	qmlRegisterType<Position>("org.ekkescorner.data", 1, 0, "Position");
	qmlRegisterType<SettingsData>("org.ekkescorner.data", 1, 0, "SettingsData");
	// register all ENUMs to get access from QML
	
	
	
}

QString DataManager::dataAssetsPath(const QString& fileName)
{
    return mDataAssetsPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}
QString DataManager::dataPath(const QString& fileName)
{
    return mDataPath + (isProductionEnvironment?PRODUCTION_ENVIRONMENT:TEST_ENVIRONMENT) + fileName;
}

bool DataManager::checkDirs()
{
    QDir myDir;
    bool exists;
    exists = myDir.exists(mDataRoot);
    if (!exists) {
        bool ok = myDir.mkpath(mDataRoot);
        if(!ok) {
            qWarning() << "Couldn't create mDataRoot " << mDataRoot;
            return false;
        }
        qDebug() << "created directory mDataRoot " << mDataRoot;
    }

    exists = myDir.exists(mDataPath);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath);
        if(!ok) {
            qWarning() << "Couldn't create mDataPath " << mDataPath;
            return false;
        }
        qDebug() << "created directory mDataPath"  << mDataRoot;
    }

    exists = myDir.exists(mDataPath+PRODUCTION_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+PRODUCTION_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/prod " << mDataPath+PRODUCTION_ENVIRONMENT;
    }
    exists = myDir.exists(mDataPath+TEST_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+TEST_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/test " << mDataPath+TEST_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory /data/test " << mDataPath+TEST_ENVIRONMENT;
    }
    return true;
}

/*
 * loads all data from cache.
 * called from main.qml with delay using QTimer
 * Data with 2PhaseInit Caching Policy will only
 * load priority records needed to resolve from others
 */
void DataManager::init()
{
    // get all from cache

    initCustomerFromCache();
    initOrderFromCache();
}



void DataManager::finish()
{
    saveCustomerToCache();
    saveOrderToCache();
    saveSettings();
}

/*
 * reads Maps of Customer in from JSON cache
 * creates List of Customer*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initCustomerFromCache()
{
	qDebug() << "start initCustomerFromCache";
    mAllCustomer.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheCustomer);
    qDebug() << "read Customer from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Customer* customer = new Customer();
        // Important: DataManager must be parent of all root DTOs
        customer->setParent(this);
        customer->fillFromCacheMap(cacheMap);
        mAllCustomer.append(customer);
    }
    qDebug() << "created Customer* #" << mAllCustomer.size();
}


/*
 * save List of Customer* to JSON cache
 * convert list of Customer* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveCustomerToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Customer* #" << mAllCustomer.size();
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        Customer* customer;
        customer = (Customer*)mAllCustomer.at(i);
        QVariantMap cacheMap;
        cacheMap = customer->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Customer* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheCustomer, cacheList);
}



/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Customer*> DataManager::listOfCustomerForKeys(
        QStringList keyList)
{
    QList<Customer*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        Customer* customer;
        customer = (Customer*) mAllCustomer.at(i);
        if (keyList.contains(QString::number(customer->nr()))) {
            listOfData.append(customer);
            keyList.removeOne(QString::number(customer->nr()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Customer: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::customerAsQVariantList()
{
    QVariantList customerList;
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        customerList.append(((Customer*) (mAllCustomer.at(i)))->toMap());
    }
    return customerList;
}

QList<QObject*> DataManager::allCustomer()
{
    return mAllCustomer;
}

QQmlListProperty<Customer> DataManager::customerPropertyList()
{
    return QQmlListProperty<Customer>(this, 0,
            &DataManager::appendToCustomerProperty, &DataManager::customerPropertyCount,
            &DataManager::atCustomerProperty, &DataManager::clearCustomerProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Customer*
void DataManager::appendToCustomerProperty(
        QQmlListProperty<Customer> *customerList,
        Customer* customer)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(customerList->object);
    if (dataManagerObject) {
        customer->setParent(dataManagerObject);
        dataManagerObject->mAllCustomer.append(customer);
        emit dataManagerObject->addedToAllCustomer(customer);
    } else {
        qWarning() << "cannot append Customer* to mAllCustomer "
                << "Object is not of type DataManager*";
    }
}
int DataManager::customerPropertyCount(
        QQmlListProperty<Customer> *customerList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(customerList->object);
    if (dataManager) {
        return dataManager->mAllCustomer.size();
    } else {
        qWarning() << "cannot get size mAllCustomer " << "Object is not of type DataManager*";
    }
    return 0;
}
Customer* DataManager::atCustomerProperty(
        QQmlListProperty<Customer> *customerList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(customerList->object);
    if (dataManager) {
        if (dataManager->mAllCustomer.size() > pos) {
            return (Customer*) dataManager->mAllCustomer.at(pos);
        }
        qWarning() << "cannot get Customer* at pos " << pos << " size is "
                << dataManager->mAllCustomer.size();
    } else {
        qWarning() << "cannot get Customer* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearCustomerProperty(
        QQmlListProperty<Customer> *customerList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(customerList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllCustomer.size(); ++i) {
            Customer* customer;
            customer = (Customer*) dataManager->mAllCustomer.at(i);
			emit dataManager->deletedFromAllCustomerByNr(customer->nr());
			emit dataManager->deletedFromAllCustomer(customer);
            customer->deleteLater();
            customer = 0;
        }
        dataManager->mAllCustomer.clear();
    } else {
        qWarning() << "cannot clear mAllCustomer " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Customer
 * and clears the list
 */
void DataManager::deleteCustomer()
{
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        Customer* customer;
        customer = (Customer*) mAllCustomer.at(i);
        emit deletedFromAllCustomerByNr(customer->nr());
		emit deletedFromAllCustomer(customer);
		emit customerPropertyListChanged();
        customer->deleteLater();
        customer = 0;
     }
     mAllCustomer.clear();
}

/**
 * creates a new Customer
 * parent is DataManager
 * if data is successfully entered you must insertCustomer
 * if edit was canceled you must undoCreateCustomer to free up memory
 */
Customer* DataManager::createCustomer()
{
    Customer* customer;
    customer = new Customer();
    customer->setParent(this);
    customer->prepareNew();
    return customer;
}

/**
 * deletes Customer
 * if createCustomer was canceled from UI
 * to delete a previous successfully inserted Customer
 * use deleteCustomer
 */
void DataManager::undoCreateCustomer(Customer* customer)
{
    if (customer) {
        qDebug() << "undoCreateCustomer " << customer->nr();
        customer->deleteLater();
        customer = 0;
    }
}

void DataManager::insertCustomer(Customer* customer)
{
    // Important: DataManager must be parent of all root DTOs
    customer->setParent(this);
    mAllCustomer.append(customer);
    emit addedToAllCustomer(customer);
    emit customerPropertyListChanged();
}

void DataManager::insertCustomerFromMap(const QVariantMap& customerMap,
        const bool& useForeignProperties)
{
    Customer* customer = new Customer();
    customer->setParent(this);
    if (useForeignProperties) {
        customer->fillFromForeignMap(customerMap);
    } else {
        customer->fillFromMap(customerMap);
    }
    mAllCustomer.append(customer);
    emit addedToAllCustomer(customer);
    customerPropertyListChanged();
}

bool DataManager::deleteCustomer(Customer* customer)
{
    bool ok = false;
    ok = mAllCustomer.removeOne(customer);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllCustomerByNr(customer->nr());
    emit deletedFromAllCustomer(customer);
    emit customerPropertyListChanged();
    customer->deleteLater();
    customer = 0;
    return ok;
}


bool DataManager::deleteCustomerByNr(const int& nr)
{
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        Customer* customer;
        customer = (Customer*) mAllCustomer.at(i);
        if (customer->nr() == nr) {
            mAllCustomer.removeAt(i);
            emit deletedFromAllCustomerByNr(nr);
            emit deletedFromAllCustomer(customer);
            emit customerPropertyListChanged();
            customer->deleteLater();
            customer = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Customer* DataManager::findCustomerByNr(const int& nr){
    for (int i = 0; i < mAllCustomer.size(); ++i) {
        Customer* customer;
        customer = (Customer*)mAllCustomer.at(i);
        if(customer->nr() == nr){
            return customer;
        }
    }
    qDebug() << "no Customer found for nr " << nr;
    return 0;
}

/*
 * reads Maps of Order in from JSON cache
 * creates List of Order*  from QVariantList
 * List declared as list of QObject* - only way to use in GroupDataModel
 */
void DataManager::initOrderFromCache()
{
	qDebug() << "start initOrderFromCache";
    mAllOrder.clear();
    QVariantList cacheList;
    cacheList = readFromCache(cacheOrder);
    qDebug() << "read Order from cache #" << cacheList.size();
    for (int i = 0; i < cacheList.size(); ++i) {
        QVariantMap cacheMap;
        cacheMap = cacheList.at(i).toMap();
        Order* order = new Order();
        // Important: DataManager must be parent of all root DTOs
        order->setParent(this);
        order->fillFromCacheMap(cacheMap);
        mAllOrder.append(order);
    }
    qDebug() << "created Order* #" << mAllOrder.size();
}


/*
 * save List of Order* to JSON cache
 * convert list of Order* to QVariantList
 * toCacheMap stores all properties without transient values
 */
void DataManager::saveOrderToCache()
{
    QVariantList cacheList;
    qDebug() << "now caching Order* #" << mAllOrder.size();
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*)mAllOrder.at(i);
        QVariantMap cacheMap;
        cacheMap = order->toCacheMap();
        cacheList.append(cacheMap);
    }
    qDebug() << "Order* converted to JSON cache #" << cacheList.size();
    writeToCache(cacheOrder, cacheList);
}


void DataManager::resolveOrderReferences(Order* order)
{
	if (!order) {
        qDebug() << "cannot resolveOrderReferences with order NULL";
        return;
    }
    if(order->isAllResolved()) {
	    qDebug() << "nothing to do: all is resolved";
	    return;
	}
    if (order->hasCustomer() && !order->isCustomerResolvedAsDataObject()) {
    	Customer* customer;
   		customer = findCustomerByNr(order->customer());
    	if (customer) {
    		order->resolveCustomerAsDataObject(customer);
    	} else {
    		qDebug() << "markCustomerAsInvalid: " << order->customer();
    		order->markCustomerAsInvalid();
    	}
    }
}

void DataManager::resolveReferencesForAllOrder()
{
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*)mAllOrder.at(i);
    	resolveOrderReferences(order);
    }
}


/**
* converts a list of keys in to a list of DataObjects
* per ex. used to resolve lazy arrays
*/
QList<Order*> DataManager::listOfOrderForKeys(
        QStringList keyList)
{
    QList<Order*> listOfData;
    keyList.removeDuplicates();
    if (keyList.isEmpty()) {
        return listOfData;
    }
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*) mAllOrder.at(i);
        if (keyList.contains(QString::number(order->nr()))) {
            listOfData.append(order);
            keyList.removeOne(QString::number(order->nr()));
            if(keyList.isEmpty()){
                break;
            }
        }
    }
    if (keyList.isEmpty()) {
        return listOfData;
    }
    qWarning() << "not all keys found for Order: " << keyList.join(", ");
    return listOfData;
}

QVariantList DataManager::orderAsQVariantList()
{
    QVariantList orderList;
    for (int i = 0; i < mAllOrder.size(); ++i) {
        orderList.append(((Order*) (mAllOrder.at(i)))->toMap());
    }
    return orderList;
}

QList<QObject*> DataManager::allOrder()
{
    return mAllOrder;
}

QQmlListProperty<Order> DataManager::orderPropertyList()
{
    return QQmlListProperty<Order>(this, 0,
            &DataManager::appendToOrderProperty, &DataManager::orderPropertyCount,
            &DataManager::atOrderProperty, &DataManager::clearOrderProperty);
}

// implementation for QQmlListProperty to use
// QML functions for List of Order*
void DataManager::appendToOrderProperty(
        QQmlListProperty<Order> *orderList,
        Order* order)
{
    DataManager *dataManagerObject = qobject_cast<DataManager *>(orderList->object);
    if (dataManagerObject) {
        order->setParent(dataManagerObject);
        dataManagerObject->mAllOrder.append(order);
        emit dataManagerObject->addedToAllOrder(order);
    } else {
        qWarning() << "cannot append Order* to mAllOrder "
                << "Object is not of type DataManager*";
    }
}
int DataManager::orderPropertyCount(
        QQmlListProperty<Order> *orderList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(orderList->object);
    if (dataManager) {
        return dataManager->mAllOrder.size();
    } else {
        qWarning() << "cannot get size mAllOrder " << "Object is not of type DataManager*";
    }
    return 0;
}
Order* DataManager::atOrderProperty(
        QQmlListProperty<Order> *orderList, int pos)
{
    DataManager *dataManager = qobject_cast<DataManager *>(orderList->object);
    if (dataManager) {
        if (dataManager->mAllOrder.size() > pos) {
            return (Order*) dataManager->mAllOrder.at(pos);
        }
        qWarning() << "cannot get Order* at pos " << pos << " size is "
                << dataManager->mAllOrder.size();
    } else {
        qWarning() << "cannot get Order* at pos " << pos
                << "Object is not of type DataManager*";
    }
    return 0;
}
void DataManager::clearOrderProperty(
        QQmlListProperty<Order> *orderList)
{
    DataManager *dataManager = qobject_cast<DataManager *>(orderList->object);
    if (dataManager) {
        for (int i = 0; i < dataManager->mAllOrder.size(); ++i) {
            Order* order;
            order = (Order*) dataManager->mAllOrder.at(i);
			emit dataManager->deletedFromAllOrderByNr(order->nr());
			emit dataManager->deletedFromAllOrder(order);
            order->deleteLater();
            order = 0;
        }
        dataManager->mAllOrder.clear();
    } else {
        qWarning() << "cannot clear mAllOrder " << "Object is not of type DataManager*";
    }
}

/**
 * deletes all Order
 * and clears the list
 */
void DataManager::deleteOrder()
{
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*) mAllOrder.at(i);
        emit deletedFromAllOrderByNr(order->nr());
		emit deletedFromAllOrder(order);
		emit orderPropertyListChanged();
        order->deleteLater();
        order = 0;
     }
     mAllOrder.clear();
}

/**
 * creates a new Order
 * parent is DataManager
 * if data is successfully entered you must insertOrder
 * if edit was canceled you must undoCreateOrder to free up memory
 */
Order* DataManager::createOrder()
{
    Order* order;
    order = new Order();
    order->setParent(this);
    order->prepareNew();
    return order;
}

/**
 * deletes Order
 * if createOrder was canceled from UI
 * to delete a previous successfully inserted Order
 * use deleteOrder
 */
void DataManager::undoCreateOrder(Order* order)
{
    if (order) {
        qDebug() << "undoCreateOrder " << order->nr();
        order->deleteLater();
        order = 0;
    }
}

void DataManager::insertOrder(Order* order)
{
    // Important: DataManager must be parent of all root DTOs
    order->setParent(this);
    mAllOrder.append(order);
    emit addedToAllOrder(order);
    emit orderPropertyListChanged();
}

void DataManager::insertOrderFromMap(const QVariantMap& orderMap,
        const bool& useForeignProperties)
{
    Order* order = new Order();
    order->setParent(this);
    if (useForeignProperties) {
        order->fillFromForeignMap(orderMap);
    } else {
        order->fillFromMap(orderMap);
    }
    mAllOrder.append(order);
    emit addedToAllOrder(order);
    orderPropertyListChanged();
}

bool DataManager::deleteOrder(Order* order)
{
    bool ok = false;
    ok = mAllOrder.removeOne(order);
    if (!ok) {
        return ok;
    }
    emit deletedFromAllOrderByNr(order->nr());
    emit deletedFromAllOrder(order);
    emit orderPropertyListChanged();
    order->deleteLater();
    order = 0;
    return ok;
}


bool DataManager::deleteOrderByNr(const int& nr)
{
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*) mAllOrder.at(i);
        if (order->nr() == nr) {
            mAllOrder.removeAt(i);
            emit deletedFromAllOrderByNr(nr);
            emit deletedFromAllOrder(order);
            emit orderPropertyListChanged();
            order->deleteLater();
            order = 0;
            return true;
        }
    }
    return false;
}


// nr is DomainKey
Order* DataManager::findOrderByNr(const int& nr){
    for (int i = 0; i < mAllOrder.size(); ++i) {
        Order* order;
        order = (Order*)mAllOrder.at(i);
        if(order->nr() == nr){
            return order;
        }
    }
    qDebug() << "no Order found for nr " << nr;
    return 0;
}







/**
 * creates a new SettingsData
 * parent is DataManager
 * if data is successfully entered you must insertSettingsData
 * if edit was canceled you must undoCreateSettingsData to free up memory
 */
SettingsData* DataManager::createSettingsData()
{
    SettingsData* settingsData;
    settingsData = new SettingsData();
    settingsData->setParent(this);
    settingsData->prepareNew();
    return settingsData;
}

/**
 * deletes SettingsData
 * if createSettingsData was canceled from UI
 * to delete a previous successfully inserted SettingsData
 * use deleteSettingsData
 */
void DataManager::undoCreateSettingsData(SettingsData* settingsData)
{
    if (settingsData) {
        qDebug() << "undoCreateSettingsData " << settingsData->id();
        settingsData->deleteLater();
        settingsData = 0;
    }
}




	

SettingsData* DataManager::settingsData()
{
    return mSettingsData;
}

void DataManager::readSettings()
{
    qDebug() << "Read the Settings File";
    mSettingsData = new SettingsData();
    mSettingsData->setParent(this);
    //
    QString assetsFilePath;

    QFile readFile(mSettingsPath);
    if(!readFile.exists()) {
        qDebug() << "settings cache doesn't exist: " << mSettingsPath;
        assetsFilePath = mDataAssetsPath+cacheSettingsData;
        QFile assetDataFile(assetsFilePath);
        if(assetDataFile.exists()) {
            // copy file from assets to data
            bool copyOk = assetDataFile.copy(mSettingsPath);
            if (!copyOk) {
                qDebug() << "cannot copy settings from data-assets to cache";
                return;
            }
            // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
            // makes the target file READ ONLY - you must set PERMISSIONS
            // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
            copyOk = readFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
            if (!copyOk) {
                qDebug() << "cannot set Permissions to read / write settings";
                return;
            }
        } else {
            qDebug() << "no settings from data-assets: " << assetsFilePath;
            return;
        }
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << mSettingsPath;
        return;
    }
    // create JSON Document from settings file
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    readFile.close();
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << mSettingsPath;
        return;
    }
    // create SettingsData* from JSON
    mSettingsData->fillFromMap(jda.toVariant().toMap());
    isProductionEnvironment = mSettingsData->isProductionEnvironment();
    qDebug() << "Settings* created";
}

void DataManager::saveSettings()
{
    qDebug() << "Save the Settings";
    // convert Settings* into JSONDocument and store to app data
    QJsonDocument jda = QJsonDocument::fromVariant(mSettingsData->toMap());
    // save JSON to data directory
    QFile saveFile(mSettingsPath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << mSettingsPath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
    saveFile.close();
    qDebug() << "SettingsData Bytes written: " << bytesWritten;
}

/*
 * reads data in from stored cache
 * if no cache found tries to get data from assets/datamodel
 */
QVariantList DataManager::readFromCache(const QString& fileName)
{
    QJsonDocument jda;
    QVariantList cacheList;
    QString cacheFilePath = dataPath(fileName);
    QFile dataFile(cacheFilePath);
    // check if already something cached
    if (!dataFile.exists()) {
        // check if there are some pre-defined data in data-assets
        QString dataAssetsFilePath = dataAssetsPath(fileName);
        qDebug() << fileName << "not found in cache" << cacheFilePath;
        qDebug() << "try copy from: " << dataAssetsFilePath;
        QFile dataAssetsFile(dataAssetsFilePath);
        if (dataAssetsFile.exists()) {
            // copy file from data-assets to cached data
            bool copyOk = dataAssetsFile.copy(cacheFilePath);
            if (!copyOk) {
                qDebug() << "cannot copy " << dataAssetsFilePath << " to " << cacheFilePath;
                return cacheList;
            }
            if(!mSettingsData->hasPublicCache()) {
                // IMPORTANT !!! copying from RESOURCES ":/data-assets/" to AppDataLocation
                // makes the target file READ ONLY - you must set PERMISSIONS
                // copying from RESOURCES ":/data-assets/" to GenericDataLocation the target is READ-WRITE
                copyOk = dataFile.setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
                if (!copyOk) {
                    qDebug() << "cannot set Permissions to read / write settings";
                    return cacheList;
                }
            }
        } else {
            // no cache, no prefilled data-assets - empty list
            return cacheList;
        }
    }
    if (!dataFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << cacheFilePath;
        return cacheList;
    }
    jda = QJsonDocument::fromJson(dataFile.readAll());
    dataFile.close();
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON Array from file: " << cacheFilePath;
        return cacheList;
    }
    cacheList = jda.toVariant().toList();
    return cacheList;
}

void DataManager::writeToCache(const QString& fileName, QVariantList& data)
{
    QString cacheFilePath = dataPath(fileName);
    QJsonDocument jda = QJsonDocument::fromVariant(data);

    QFile saveFile(cacheFilePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file to write " << cacheFilePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson(mCompactJson?QJsonDocument::Compact:QJsonDocument::Indented));
    saveFile.close();
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << cacheFilePath;
}

DataManager::~DataManager()
{
    // clean up
}
