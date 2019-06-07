#!/bin/sh

g++ -std=c++11 ./Source/*.cpp ./Source/Common/*.cpp -o Tetris -I ../Deps/freetype-2.10.0/include/ -lGLEW -lglfw -lfreetype -lGL