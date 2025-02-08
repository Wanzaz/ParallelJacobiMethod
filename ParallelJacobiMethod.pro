QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/argumentparser.cpp \
        src/jacobisolver.cpp \
        src/jacobiworker.cpp \
        src/main.cpp \
        src/matrixhandler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/argumentparser.h \
    src/jacobisolver.h \
    src/jacobiworker.h \
    src/matrixhandler.h
