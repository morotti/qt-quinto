PRECOMPILED_HEADER = src/precompiled.h

HEADERS += \
    src/board.h \
    src/precompiled.h \
    src/BoardModel.h

SOURCES += \
    src/board.cpp \
    src/BoardModel.cpp \
    tests/tutmain.cpp \
    tests/tuttest.cpp \
    tests/BoardTest.cpp

FORMS +=

RESOURCES +=

QT += widgets

INCLUDEPATH += C:\boost_1_56_0
INCLUDEPATH += C:\tut-framework-2013-12-18\include
INCLUDEPATH += src

QMAKE_CXXFLAGS += -openmp

TARGET = tests

#----------
CONFIG(release, debug|release) {
    SUBDIR = release
} else {
    SUBDIR = debug
}

#----------
win32 {
    IN_EXE = $${OUT_PWD}\\$${SUBDIR}\\$${TARGET}.exe
    IN_EXE ~= s,/,\\,g

    QMAKE_POST_LINK += $${IN_EXE}
}
