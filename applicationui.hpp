// ekke (Ekkehard Gentz) @ekkescorner
#ifndef APPLICATIONUI_HPP
#define APPLICATIONUI_HPP

#include <QObject>

#include "SettingsData.hpp"

class ApplicationUI : public QObject
{
    Q_OBJECT

public:
     ApplicationUI(QObject *parent = 0);

     Q_INVOKABLE
     QStringList swapThemePalette();

     Q_INVOKABLE
     QStringList defaultThemePalette();

     Q_INVOKABLE
     QStringList primaryPalette(const int paletteIndex);

     Q_INVOKABLE
     QStringList accentPalette(const int paletteIndex);

     Q_INVOKABLE
     QStringList defaultPrimaryPalette();

     Q_INVOKABLE
     QStringList defaultAccentPalette();


signals:

public slots:
     void onAboutToQuit();
     void onApplicationStateChanged(Qt::ApplicationState applicationState);

private:
     QString mDataPath;
     bool checkDirs();

     SettingsData* mSettingsData;
     bool readSettings();
     void saveSettings();
};

#endif // APPLICATIONUI_HPP
