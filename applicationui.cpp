// ekke (Ekkehard Gentz) @ekkescorner
#include "applicationui.hpp"
#include "uiconstants.hpp"

#include <QtQml>
#include <QGuiApplication>

#include <QJsonObject>
#include <QFile>


#include <QDebug>

const QString settingsDataFile = "/settingsData.json";

using namespace ekke::constants;

ApplicationUI::ApplicationUI(QObject *parent) : QObject(parent)
{
    bool ok = checkDirs();
    if(!ok) {
        qFatal("App won't work - cannot create data directory");
    }

    qmlRegisterType<SettingsData>("org.ekkescorner.data", 1, 0, "SettingsData");

    mSettingsData = new SettingsData();
    // Read settings from data cache - if not found use defaults
    if(!readSettings()) {
        // default theme is light
        mSettingsData->setDarkTheme(false);
        // default primary color is Teal
        mSettingsData->setPrimaryColor(8);
        // default accent color is DeepOrange
        mSettingsData->setAccentColor(15);
    }
}

/* Change Theme Palette */
QStringList ApplicationUI::swapThemePalette()
{
    mSettingsData->setDarkTheme(!mSettingsData->darkTheme());
    if (mSettingsData->darkTheme()) {
        return darkPalette;
    }
    return lightPalette;
}

/* Get current default Theme Palette */
QStringList ApplicationUI::defaultThemePalette()
{
    if (mSettingsData->darkTheme()) {
        return darkPalette;
    }
    return lightPalette;
}

/* Get one of the Primary Palettes */
QStringList ApplicationUI::primaryPalette(const int paletteIndex)
{
    mSettingsData->setPrimaryColor(paletteIndex);
    switch (paletteIndex) {
    case 0:
        return materialRed;
        break;
    case 1:
        return materialPink;
        break;
    case 2:
        return materialPurple;
        break;
    case 3:
        return materialDeepPurple;
        break;
    case 4:
        return materialIndigo;
        break;
    case 5:
        return materialBlue;
        break;
    case 6:
        return materialLightBlue;
        break;
    case 7:
        return materialCyan;
        break;
    case 8:
        return materialTeal;
        break;
    case 9:
        return materialGreen;
        break;
    case 10:
        return materialLightGreen;
        break;
    case 11:
        return materialLime;
        break;
    case 12:
        return materialYellow;
        break;
    case 13:
        return materialAmber;
        break;
    case 14:
        return materialOrange;
        break;
    case 15:
        return materialDeepOrange;
        break;
    case 16:
        return materialBrown;
        break;
    case 17:
        return materialGrey;
        break;
    default:
        return materialBlueGrey;
        break;
    }
}

/* Get one of the Accent Palettes */
QStringList ApplicationUI::accentPalette(const int paletteIndex)
{
    mSettingsData->setAccentColor(paletteIndex);
    int currentPrimary = mSettingsData->primaryColor();
    QStringList thePalette = primaryPalette(paletteIndex);
    mSettingsData->setPrimaryColor(currentPrimary);
    // we need: primaryColor, textOnPrimary, iconOnPrimaryFolder
    return QStringList{thePalette.at(1), thePalette.at(4), thePalette.at(7)};
}

/* Get Default Primary Palette */
QStringList ApplicationUI::defaultPrimaryPalette()
{
    return primaryPalette(mSettingsData->primaryColor());
}

/* Get Default Accent Palette */
QStringList ApplicationUI::defaultAccentPalette()
{
    return accentPalette(mSettingsData->accentColor());
}

SettingsData *ApplicationUI::getSettings()
{
    return mSettingsData;
}

// ATTENTION
// iOS: NO SIGNAL
// Android: SIGNAL if leaving the App with Android BACK Key
// Android: NO SIGNAL if using HOME or OVERVIEW and THEN CLOSE from there
void ApplicationUI::onAboutToQuit()
{
    saveSettings();
    qDebug() << "Q U I T";
}

void ApplicationUI::onApplicationStateChanged(Qt::ApplicationState applicationState)
{
    if(applicationState == Qt::ApplicationState::ApplicationSuspended) {
        saveSettings();
    }
    qDebug() << "S T A T E: " << applicationState;
}

bool ApplicationUI::checkDirs()
{
    // Android: HomeLocation works, iOS: not writable
    // Android: AppDataLocation works out of the box, iOS you must create the DIR first !!
    mDataPath = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);
    mDataPath += "/data";
    qDebug() << "Data Path: " << mDataPath;
    QDir myDir(mDataPath);
    if (!myDir.exists()) {
        bool ok = myDir.mkpath(mDataPath);
        if(!ok) {
            qWarning() << "Couldn't create dir. " << mDataPath;
            return false;
        }
        qDebug() << "created directory path" << mDataPath;
    }
    return true;
}

bool ApplicationUI::readSettings()
{
    qDebug() << "Read the Settings File";
    QFile readFile(mDataPath+settingsDataFile);
    if(!readFile.exists()) {
        qDebug() << "file doesn't exist: " << settingsDataFile;
        return false;
    }
    if (!readFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file: " << settingsDataFile;
        return false;
    }
    // create JSON Document from settings file
    QJsonDocument jda = QJsonDocument::fromJson(readFile.readAll());
    if(!jda.isObject()) {
        qWarning() << "Couldn't create JSON from file: " << settingsDataFile;
        return false;
    }
    // create SettingsData* from JSON
    mSettingsData->fillFromMap(jda.toVariant().toMap());
    return true;
}

void ApplicationUI::saveSettings()
{
    qDebug() << "Save the Settings";
    // convert Settings* into JSONDocument and store to app data
    QJsonDocument jda = QJsonDocument::fromVariant(mSettingsData->toMap());
    // save JSON to data directory
    QFile saveFile(mDataPath+settingsDataFile);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Couldn't open file " << settingsDataFile;
        return;
    }
    qint64 bytesWritten = saveFile.write(jda.toJson());
    qDebug() << "Bytes written: " << bytesWritten;
}



