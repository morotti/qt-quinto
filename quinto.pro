PRECOMPILED_HEADER = src/precompiled.h

HEADERS += \
    src/board.h \
    src/gui.h \
    src/precompiled.h \
    src/solver.h \
    src/BoardModel.h

SOURCES += \
    src/board.cpp \
    src/gui.cpp \
    src/main.cpp \
    src/solver.cpp \
    src/BoardModel.cpp

FORMS += \
    src/gui.ui

RESOURCES += \
    src/gui.qrc

QT += widgets

win32:INCLUDEPATH += C:\boost_1_56_0

QMAKE_CXXFLAGS += -openmp

TARGET = quinto

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
}

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
