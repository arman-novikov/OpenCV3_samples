TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lopencv_core
LIBS += -lopencv_highgui
LIBS += -lopencv_imgcodecs
LIBS += -lopencv_imgproc
LIBS += -lopencv_videoio
LIBS += -lopencv_video
LIBS += -lopencv_videostab
LIBS += -lopencv_shape

QMAKE_LFLAGS += -no-pie
QMAKE_LFLAGS += -pthread

SOURCES += \
        main.cpp \
    src/Chapter1.cpp \
    src/Chapter2.cpp \
    src/Chapter3.cpp \
    src/Chapter4.cpp \
    src/Chapter5.cpp \
    src/Chapter6.cpp \
    src/MetaChapter.cpp \
    src/Chapter7.cpp \
    src/Chapter8.cpp

HEADERS += \
    include/Chapter1.h \
    include/Chapter2.h \
    include/Chapter3.h \
    include/Chapter4.h \
    include/Chapter5.h \
    include/Chapter6.h \
    include/MetaChapter.h \
    include/Chapter7.h \
    include/Chapter8.h
