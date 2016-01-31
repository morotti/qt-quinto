#Qt Quinto
[![Build Status](https://travis-ci.org/morotti/qt-quinto.svg?branch=master)](https://travis-ci.org/morotti/qt-quinto)
[![Coverage Status](https://coveralls.io/repos/morotti/qt-quinto/badge.svg?branch=master&service=github)](https://coveralls.io/github/morotti/qt-quinto?branch=master)
[![Build status](https://ci.appveyor.com/api/projects/status/4cf94oi69i11ecn7?svg=true)](https://ci.appveyor.com/project/morotti/qt-quinto)

## The Game
### Gameplay
![Picture of the board](https://github.com/morotti/qt-quinto/blob/master/img/board_4x4.png "4x4 quinto board example")

The quinto game is played on a black and white board. Click on a cell to reverse its color and the colors of its neighbors. All cells are white at the beginning. Turn them all black to win.

### Solver
Can't find the solution ? Just click "Solve" and wonder at the computer thinking for you.<br/>
Too boring to click on all these cells ? Just click "Ai" and be amazed at the computer playing for you.

<img src="https://github.com/morotti/qt-quinto/blob/master/img/solver_50x50.gif?raw=true" width="640" height="368" alt="Video of the solver on a 50x50 board">

(click the picture to see it full screen)

## Behind the scenes
### Where does this game come from ?
I have no clue ! I originally made it circa 2010 for a school assignment from the C course "make anything you like aka. preferably a fun video game using C/SDL/Code:Blocks". I showed it to other students and one said it was called the "quinto" game.

I use this game to try new technologies ever since. It has gone a long way. You can check the online version at https://the-quinto-game.appspot.com/

### Solver Algorithm
The board is converted into a set of matrices representing the current state, the links between cells and the state you want to reach (all black). Mathematically speaking those matrices are a set of equations which is solved using linear algebra (gauss pivot). 

For instance a 50x50 board generates a set of 2500 equations, each with 2500 variables.

### Memory and Speed Optimizations
Given the side of the board is n. The solver complexity is n^4 in memory and n^4 in time. It takes some optimizations to run on current hardware for very large board:
* Intel MMX then Intel SSE2 for faster speed (math operations on 64 then 128 bits at a time)
* OpenMP for even faster speed (multi-core support)
* Boost:UBLAS for memory saving (binary variables stored as single bits rather than entire bytes)
* Qt-related optimizations for faster visual rendering (found and removed hotspots using the visual studio ultimate profiler)

The current version can solve a 300x300 board (90000x90000 equations aka 8.1 BILLION elements matrix) in less than 1 minute and 1 GB memory, then display the animation before your eyes.

## Tools
### Build
Build can be initiated from Qt Creator (on your computer), from a cloud CI server or from a command prompt. Build configuration is self-contained in the .pro files.

Building will create subfolders "bin/OS-ARCH-QTVERSION-COMPILER" and copy final executables with their dependencies upon success.

### Packaging
A windows installer is packaged using Inno Setup. It is available in /bin/

### Unit Testing
The project has a bunch of unit tests running on the TUT framework (a simple cross-platform & header-only unit test framework).

Qt can only make a single executable per project so there is a separate project "tests.pro" for unit tests.

### Code Coverage
Code coverage is analyzed using gcov on linux. Coverage report is sent to coveralls upon completion.

gcov requires gcc (linux only) and lcov version >= 1.12 (previous versions lack important flags and can be buggy at times)

### Continuous Integration
#### Travis-CI
* Build on linux | gcc
* 64 bits only
* Run unit tests (with coverage)
* Upload coverage report to coveralls

### AppVeyor
* Build on Server2012 | VS 2013 Express | Qt 5.3
* Both 32 and 64 bits
* Run unit tests

#### Teamcity (personal local server)
* Build on Win7 | VS 2010 Ultimate, VS 2012 Ultimate, VS 2013 Ultimate
* Both 32 and 64 bits
* Run unit tests
* Package the windows installer
* Store build artefacts (exes and installers)

### Potential Future Improvements
* Use AVX instructions for even faster equation solving !
* Try the clang compiler
* Try clang code coverage (can't be worse than gcov)
* Package a MSI installer using WIX
