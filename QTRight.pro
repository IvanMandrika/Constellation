QT += core gui widgets

CONFIG += c++20
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = Constellation

SOURCES += \
    main.cpp \
    ConstellationWidget.cpp \
    DataReader.cpp

HEADERS += \
    ConstellationWidget.h \
    DataReader.h

RESOURCES +=

extra.target = copy_files
extra.commands = cp data/A16.xml data/128.xml data/256.xml data/32.xml data/iq_qam32.bin $$OUT_PWD/
QMAKE_EXTRA_TARGETS += extra
