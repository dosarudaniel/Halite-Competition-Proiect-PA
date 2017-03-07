#!/bin/bash

g++ -std=c++11 MyBot.cpp -o MyBot.o
g++ -std=c++11 RandomBot.cpp -o RandomBot.o
./halite -d "50 50" -s 4202324323 "./MyBot.o" 
