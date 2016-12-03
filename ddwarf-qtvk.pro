TEMPLATE = lib
VERSION = 1.0
TARGET = ddwarf-qtvk

QT += qml quick

CONFIG += c++11
INCLUDEPATH +=

macx: QMAKE_SONAME_PREFIX = @executable_path/../Frameworks

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
PATCHES = $$files(patches/*)

OTHER_FILES += \
    LICENSE \
    README.md \
    $$PATCHES
    $$JAVA_FILES

include(pvs-studio.pri)
