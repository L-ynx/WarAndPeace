@echo off

REM Set the compiler and compiler flags
set CXX=g++
set CXXFLAGS=-std=c++17

REM Compile the main application functions
%CXX% %CXXFLAGS% src/war_and_peace.cpp -o main.exe

REM Compile the test files
%CXX% %CXXFLAGS% -c test/tests.cpp -o tests.o
%CXX% %CXXFLAGS% test/test_main.cpp tests.o -o tests.exe

REM Run the tests
tests.exe

REM Run the main application
main.exe

REM Pause to keep the command prompt open after running
pause
