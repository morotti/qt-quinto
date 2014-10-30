set BASEPATH=%PATH%

del MakeFile*
rmdir /s /q debug
rmdir /s /q release

rem ----------
set PATH=C:\Qt\Tools\MinGW\bin\;%BASEPATH%

C:\Qt\5.3\mingw482_32\bin\qmake.exe -spec win32-g++ tests.pro
mingw32-make.exe --always-make -f Makefile all

mingw32-make.exe clean
set PATH=%BASEPATH%
del MakeFile*
rmdir /s /q debug
rmdir /s /q release

rem ----------
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86 
C:\Qt\5.3\msvc2013_opengl\bin\qmake.exe -spec win32-msvc2013 tests.pro
nmake -b -f Makefile all

nmake clean
del MakeFile*
rmdir /s /q debug
rmdir /s /q release

rem ----------
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64
C:\Qt\5.3\msvc2013_64_opengl\bin\qmake.exe -spec win32-msvc2013 tests.pro
nmake -b -f Makefile all

nmake clean
del MakeFile*
rmdir /s /q debug
rmdir /s /q release

rem ----------
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
C:\Qt\5.3\msvc2012_opengl\bin\qmake.exe -spec win32-msvc2012 tests.pro
nmake -b -f Makefile all

nmake clean
del MakeFile*
rmdir /s /q debug
rmdir /s /q release
