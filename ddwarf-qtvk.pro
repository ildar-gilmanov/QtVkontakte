TEMPLATE = lib
VERSION = 1.0
TARGET = ddwarf-qtvk

QT += qml quick

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
    QtVk.cpp

android {
    QT += androidextras
    SOURCES += QtVk-android.cpp
} else {
    SOURCES += \
        QtVk-desktop.cpp
}

JAVA_FILES = android/src/org/ddwarf/vk/QtVkBinding.java

OTHER_FILES += \
    $$JAVA_FILES
