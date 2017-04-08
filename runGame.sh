#!/bin/bash


# Stage 2
# Objective: Beat DBot_linux_64bit

# Înălțime hartă	Lungime hartă	Seed	Punctaj
#	28					24			314			0.1
#	30					30			 42			0.1
#	40					40			154			0.1
#	30					50			  3			0.1
# 	50					50			 42			0.1

g++ -std=c++11 BestBot.cpp -o BestBot
./halite -d "28 24" -s 314 "./BestBot" "./DBot_linux_64bit" 
./halite -d "30 30" -s 42 "./BestBot" "./DBot_linux_64bit" 
./halite -d "40 40" -s 154 "./DBot_linux_64bit" "./BestBot"
./halite -d "30 50" -s 3 "./DBot_linux_64bit" "./BestBot"
./halite -d "50 50" -s 42 "./DBot_linux_64bit" "./BestBot"
# Uncomment if you to see how good is DBot_linux_64bit 
#./halite -d "50 50" -s 42 "./DBot_linux_64bit"
