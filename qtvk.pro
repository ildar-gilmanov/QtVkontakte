TEMPLATE = lib
VERSION = 1.0
TARGET = qtvk

QT += qml

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

    JAVA_FILES.path = android/src/org/ddwarf/vk/
    JAVA_FILES.files += $$files(android/src/org/ddwarf/vk/*)

    INSTALLS += JAVA_FILES

    OTHER_FILES += \
        $$JAVA_FILES

} else {
    SOURCES += \
        QtVk-desktop.cpp
}
