g++ -c -Wall GPS_functions.cpp
g++ -c -Wall gps.cpp
g++ gps.o GPS_functions.o -o gps -lwiringPi -pthread
