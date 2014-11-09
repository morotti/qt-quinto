PRECOMPILED_HEADER = src/precompiled.h

HEADERS += \
    src/board.h \
    src/precompiled.h \
    src/BoardModel.h \
    src/solver.h

SOURCES += \
    src/board.cpp \
    src/BoardModel.cpp \
    tests/tutmain.cpp \
    tests/tuttest.cpp \
    tests/BoardTest.cpp \
    tests/SolverTest.cpp \
    src/solver.cpp

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
win32-msvc2012 {
    COMPILER = msvc2012
}

win32-msvc2013 {
    COMPILER = msvc2013
}

win32-g++ {
    COMPILER = mingw
}

#----------
win32 {
    IN_EXE = $${OUT_PWD}\\$${SUBDIR}\\$${TARGET}.exe
    IN_EXE ~= s,/,\\,g

    OUT_DIR = $${IN_PWD}\\bin\\win-$${QT_ARCH}-$${COMPILER}-$${SUBDIR}
    OUT_EXE = $${OUT_DIR}\\$${TARGET}.exe
    OUT_DIR ~= s,/,\\,g
    OUT_EXE ~= s,/,\\,g

    QMAKE_POST_LINK += (if not exist $${OUT_DIR} ( mkdir $${OUT_DIR} ))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += copy /Y $${IN_EXE} $${OUT_EXE}
}
