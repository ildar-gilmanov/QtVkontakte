TEMPLATE = lib
VERSION = 1.0
TARGET = qtvk

QT += qml
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


HEADERS += \
    QtVk.h

SOURCES += \
    QtVk.cpp \
    QtVk-android.cpp

JAVA_FILES = android/src/org/ddwarf/vk/QtVkBinding.java

OTHER_FILES += \
    $$JAVA_FILES
