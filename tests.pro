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

win32:INCLUDEPATH += C:\boost_1_56_0
win32:INCLUDEPATH += C:\tut-framework-2013-12-18\include
unix:INCLUDEPATH  += $$(HOME)/lib/tut/include
INCLUDEPATH += src

QMAKE_CXXFLAGS += -openmp
unix:QMAKE_CXXFLAGS += --coverage
unix:QMAKE_LFLAGS += --coverage
# --coverage implies -fprofile-arcs -ftest-coverage and/or -lgcov wherever appropriate

TARGET = tests
unix:TARGET = tests.bin

#----------
CONFIG(release, debug|release) {
    SUBDIR = release
} else {
    SUBDIR = debug
}

#----------
linux-g++ {
    COMPILER = g++
}

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
unix {
    IN_EXE = $${OUT_PWD}/$${TARGET}

    OUT_DIR = $${IN_PWD}/bin/unix-$${QT_ARCH}-$${COMPILER}-$${SUBDIR}
    OUT_EXE = $${OUT_DIR}/$${TARGET}

    QMAKE_POST_LINK += mkdir -p $${OUT_DIR}
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += cp -f $${IN_EXE} $${OUT_EXE}
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $${IN_EXE}
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += lcov --capture --directory . --output-file coverage.info --no-external
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += coveralls-lcov coverage.info
}

win32 {
    IN_EXE = $${OUT_PWD}\\$${SUBDIR}\\$${TARGET}.exe
    IN_EXE ~= s,/,\\,g

    OUT_DIR = $${IN_PWD}\\bin\\$${QMAKE_PLATFORM}-$${QT_ARCH}-qt$${QT_MAJOR_VERSION}.$${QT_MINOR_VERSION}-$${COMPILER}-$${SUBDIR}
    OUT_EXE = $${OUT_DIR}\\$${TARGET}.exe
    OUT_DIR ~= s,/,\\,g
    OUT_EXE ~= s,/,\\,g

    QMAKE_POST_LINK += (if not exist $${OUT_DIR} ( mkdir $${OUT_DIR} ))
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += copy /Y $${IN_EXE} $${OUT_EXE}
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += $${IN_EXE}
    QMAKE_POST_LINK += &&
    QMAKE_POST_LINK += windeployqt --force --no-webkit2 --no-system-d3d-compiler --no-translations --no-quick-import $${OUT_EXE}
}
