TEMPLATE = lib
VERSION = 1.0
TARGET = qtvk

QT +=
android:QT += androidextras

CONFIG += c++11
INCLUDEPATH +=

isEmpty(PREFIX) {
    PREFIX = /usr/local/
}


unix:!android {
    target.path = $$(PREFIX)

    INSTALLS += target
}


HEADERS +=

SOURCES +=

OTHER_FILES +=
