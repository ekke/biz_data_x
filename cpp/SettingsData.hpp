#ifndef SETTINGSDATA_HPP_
#define SETTINGSDATA_HPP_

#include <QObject>
#include <qvariant.h>




class SettingsData: public QObject
{
	Q_OBJECT

	Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged FINAL)
	Q_PROPERTY(int version READ version WRITE setVersion NOTIFY versionChanged FINAL)
	Q_PROPERTY(bool isProductionEnvironment READ isProductionEnvironment WRITE setIsProductionEnvironment NOTIFY isProductionEnvironmentChanged FINAL)
	Q_PROPERTY(int primaryColor READ primaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged FINAL)
	Q_PROPERTY(int accentColor READ accentColor WRITE setAccentColor NOTIFY accentColorChanged FINAL)
	Q_PROPERTY(bool darkTheme READ darkTheme WRITE setDarkTheme NOTIFY darkThemeChanged FINAL)


public:
	SettingsData(QObject *parent = 0);


	void fillFromMap(const QVariantMap& settingsDataMap);
	void fillFromForeignMap(const QVariantMap& settingsDataMap);
	void fillFromCacheMap(const QVariantMap& settingsDataMap);
	
	void prepareNew();
	
	bool isValid();

	Q_INVOKABLE
	QVariantMap toMap();
	QVariantMap toForeignMap();
	QVariantMap toCacheMap();

	int id() const;
	void setId(int id);
	int version() const;
	void setVersion(int version);
	bool isProductionEnvironment() const;
	void setIsProductionEnvironment(bool isProductionEnvironment);
	int primaryColor() const;
	void setPrimaryColor(int primaryColor);
	int accentColor() const;
	void setAccentColor(int accentColor);
	bool darkTheme() const;
	void setDarkTheme(bool darkTheme);



	virtual ~SettingsData();

	Q_SIGNALS:

	void idChanged(int id);
	void versionChanged(int version);
	void isProductionEnvironmentChanged(bool isProductionEnvironment);
	void primaryColorChanged(int primaryColor);
	void accentColorChanged(int accentColor);
	void darkThemeChanged(bool darkTheme);
	

private:

	int mId;
	int mVersion;
	bool mIsProductionEnvironment;
	int mPrimaryColor;
	int mAccentColor;
	bool mDarkTheme;

	Q_DISABLE_COPY (SettingsData)
};
Q_DECLARE_METATYPE(SettingsData*)

#endif /* SETTINGSDATA_HPP_ */

