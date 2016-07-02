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
    mDataAssetsPath = "qrc:///data-assets/";
    qDebug() << "Data Path: " << mDataPath << " data-assets: " << mDataAssetsPath;
    // guarantee that dirs exist
    bool ok = checkDirs();
    if(!ok) {
        qFatal("App won't work - cannot create data directory");
    }

    // ApplicationUI is parent of DataManager
    // DataManager is parent of all root DataObjects
    // ROOT DataObjects are parent of contained DataObjects
    // ROOT:
    // Customer
    // Order
    // SettingsData
    readSettings();
    isProductionEnvironment = mSettingsData->isProductionEnvironment();

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
            qWarning() << "Couldn't create /data dir. " << mDataRoot;
            return false;
        }
        qDebug() << "created directory path" << mDataRoot;
    }
    exists = myDir.exists(mDataPath+PRODUCTION_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+PRODUCTION_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/prod dir. " << mDataPath+PRODUCTION_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory path" << mDataPath+PRODUCTION_ENVIRONMENT;
    }
    exists = myDir.exists(mDataPath+TEST_ENVIRONMENT);
    if (!exists) {
        bool ok = myDir.mkpath(mDataPath+TEST_ENVIRONMENT);
        if(!ok) {
            qWarning() << "Couldn't create /data/test dir. " << mDataPath+TEST_ENVIRONMENT;
            return false;
        }
        qDebug() << "created directory path" << mDataPath+TEST_ENVIRONMENT;
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
    //
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

// implementation for QDeclarativeListProperty to use
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
            customer->deleteLater();
            customer = 0;
            return true;
        }
    }
    return false;
}

//void DataManager::fillCustomerDataModel(QString objectName)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            QList<QObject*> theList;
//            for (int i = 0; i < mAllCustomer.size(); ++i) {
//                theList.append(mAllCustomer.at(i));
//            }
//            dataModel->clear();
//            dataModel->insertList(theList);
//            return;
//        }
//    }
//    qDebug() << "NO GRP DATA FOUND Customer for " << objectName;
//}
///**
// * removing and re-inserting a single item of a DataModel
// * this will cause the ListView to redraw or recalculate all values for this ListItem
// * we do this, because only changing properties won't call List functions
// */
//void DataManager::replaceItemInCustomerDataModel(QString objectName,
//                                                 Customer* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            bool exists = dataModel->remove(listItem);
//            if (exists) {
//                dataModel->insert(listItem);
//                return;
//            }
//            qDebug() << "Customer Object not found and not replaced in " << objectName;
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

//void DataManager::removeItemFromCustomerDataModel(QString objectName, Customer* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            bool exists = dataModel->remove(listItem);
//            if (exists) {
//                return;
//            }
//            qDebug() << "Customer Object not found and not removed from " << objectName;
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

//void DataManager::insertItemIntoCustomerDataModel(QString objectName, Customer* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            dataModel->insert(listItem);
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

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

// implementation for QDeclarativeListProperty to use
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
            order->deleteLater();
            order = 0;
            return true;
        }
    }
    return false;
}

//void DataManager::fillOrderDataModel(QString objectName)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            QList<QObject*> theList;
//            for (int i = 0; i < mAllOrder.size(); ++i) {
//                theList.append(mAllOrder.at(i));
//            }
//            dataModel->clear();
//            dataModel->insertList(theList);
//            return;
//        }
//    }
//    qDebug() << "NO GRP DATA FOUND Order for " << objectName;
//}
///**
// * removing and re-inserting a single item of a DataModel
// * this will cause the ListView to redraw or recalculate all values for this ListItem
// * we do this, because only changing properties won't call List functions
// */
//void DataManager::replaceItemInOrderDataModel(QString objectName,
//                                              Order* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            bool exists = dataModel->remove(listItem);
//            if (exists) {
//                dataModel->insert(listItem);
//                return;
//            }
//            qDebug() << "Order Object not found and not replaced in " << objectName;
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

//void DataManager::removeItemFromOrderDataModel(QString objectName, Order* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            bool exists = dataModel->remove(listItem);
//            if (exists) {
//                return;
//            }
//            qDebug() << "Order Object not found and not removed from " << objectName;
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

//void DataManager::insertItemIntoOrderDataModel(QString objectName, Order* listItem)
//{
//    // using dynamic created Pages / Lists it's a good idea to use findChildren ... last()
//    // probably there are GroupDataModels not deleted yet from previous destroyed Pages
//    QList<GroupDataModel*> dataModelList = Application::instance()->scene()->findChildren<
//            GroupDataModel*>(objectName);
//    if (dataModelList.size() > 0) {
//        GroupDataModel* dataModel = dataModelList.last();
//        if (dataModel) {
//            dataModel->insert(listItem);
//        }
//        return;
//    }
//    qDebug() << "no DataModel found for " << objectName;
//}

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
    QString cacheFilePath;
    QString assetsFilePath;
    cacheFilePath = mDataRoot+"/"+cacheSettingsData;

    QFile readFile(cacheFilePath);
    if(!readFile.exists()) {
        qDebug() << "settings cache doesn't exist: " << cacheFilePath;
        assetsFilePath = mDataAssetsPath+cacheSettingsData;
        QFile assetDataFile(assetsFilePath);
        if(assetDataFile.exists()) {
            // copy file from assets to data
            bool copyOk = assetDataFile.copy(cacheFilePath);
            if (!copyOk) {
                qDebug() << "cannot copy settings from assets to cache";
                // no cache, no assets - empty settings
                return;
            }
        } else {
            // no cache, no assets - empty settings
            qDebug() << "no settings from assets: " << assetsFilePath;
            return;
        }
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << cacheFilePath;
        return;
    }
    // create JSON Document from settings file
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << cacheFilePath;
        return;
    }
    // create SettingsData* from JSON
    mSettingsData->fillFromMap(jda.toVariant().toMap());
    qDebug() << "Settings* created";
}

void DataManager::saveSettings()
{
    qDebug() << "Save the Settings";
    // convert Settings* into JSONDocument and store to app data
    QJsonDocument jda = QJsonDocument::fromVariant(mSettingsData->toMap());
    // save JSON to data directory
    QFile saveFile(mDataRoot+"/"+cacheSettingsData);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file " << cacheSettingsData;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
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
    QString assetsFilePath = dataAssetsPath(fileName);
    QFile dataFile(cacheFilePath);
    if (!dataFile.exists()) {
        QFile assetDataFile(assetsFilePath);
        if (assetDataFile.exists()) {
            // copy file from assets to data
            bool copyOk = assetDataFile.copy(cacheFilePath);
            if (!copyOk) {
                qDebug() << "cannot copy dataAssetsPath(fileName) to dataPath(fileName)";
                // no cache, no assets - empty list
                return cacheList;
            }
        } else {
            // no cache, no assets - empty list
            return cacheList;
        }
    }
    jda = QJsonDocument::fromJson(dataFile.readAll());
    if(!jda.isArray()) {
        qWarning() << "Couldn't create JSON[] from file: " << cacheFilePath;
        return cacheList;
    }
    cacheList = jda.toVariant().toList();
    return cacheList;
}

void DataManager::writeToCache(const QString& fileName, QVariantList& data)
{
    QString filePath = dataPath(fileName);
    QJsonDocument jda = QJsonDocument::fromVariant(data);
    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file " << filePath;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
    qDebug() << "Data Bytes written: " << bytesWritten << " to: " << filePath;
}

void DataManager::onManualExit()
{
    qDebug() << "## DataManager ## MANUAL EXIT";
    finish();
}

DataManager::~DataManager()
{
    // clean up
}
