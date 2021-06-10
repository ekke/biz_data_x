# ekke (Ekkehard Gentz) @ekkescorne
TEMPLATE = app
TARGET = biz_data_x

QT += qml quick core
CONFIG += c++11

HEADERS += \
    cpp/applicationui.hpp \
    cpp/uiconstants.hpp \
    cpp/gen/SettingsData.hpp \
    cpp/gen/Customer.hpp \
    cpp/gen/DataManager.hpp \
    cpp/gen/Order.hpp \
    cpp/gen/Position.hpp

SOURCES += cpp/main.cpp \
    cpp/applicationui.cpp \
    cpp/gen/SettingsData.cpp \
    cpp/gen/Customer.cpp \
    cpp/gen/DataManager.cpp \
    cpp/gen/Order.cpp \
    cpp/gen/Position.cpp

lupdate_only {
    SOURCES +=  qml/main.qml \
    qml/common/*.qml \
    qml/navigation/*.qml \
    qml/pages/*.qml \
    qml/popups/*.qml \
    qml/tabs/*.qml
}

OTHER_FILES += images/black/*.png \
    images/black/x18/*.png \
    images/black/x36/*.png \
    images/black/x48/*.png \
    images/white/*.png \
    images/white/x18/*.png \
    images/white/x36/*.png \
    images/white/x48/*.png \
    images/extra/*.png \
    translations/*.* \
    data-assets/*.json \
	data-assets/prod/*.json \
	data-assets/test/*.json \
    images/LICENSE \
    LICENSE \
    *.md

RESOURCES += qml.qrc \
    translations.qrc \
    images.qrc \
    data-assets.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# T R A N S L A T I O N S

# if languages are added:
# 1. rebuild project to generate *.qm
# 2. add existing .qm files to translations.qrc

# if changes to translatable strings:
# 1. Run Tools-External-Linguist-Update
# 2. Run Linguist and do translations
# 3. Build and run on iOS and Android to verify translations
# 4. Optional: if translations not done: Run Tools-External-Linguist-Release

# Supported languages
LANGUAGES = de_at de en

# used to create .ts files
 defineReplace(prependAll) {
     for(a,$$1):result += $$2$${a}$$3
     return($$result)
 }
# Available translations
tsroot = $$join(TARGET,,,.ts)
tstarget = $$join(TARGET,,,_)
TRANSLATIONS = $$PWD/translations/$$tsroot
TRANSLATIONS += $$prependAll(LANGUAGES, $$PWD/translations/$$tstarget, .ts)
# run LRELEASE to generate the qm files
qtPrepareTool(LRELEASE, lrelease)
 for(tsfile, TRANSLATIONS) {
     command = $$LRELEASE $$tsfile
     system($$command)|error("Failed to run: $$command")
 }

DISTFILES += \
    gen-model/README.md \
    gen-model/*.pdf \
    gen-model/*.txt
