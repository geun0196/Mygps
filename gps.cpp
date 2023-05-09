#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
#include <wiringSerial.h>

#include "GPSData.h"
#include "GPS_functions.h"

using namespace std;

// Global variables for the two threads to communicate
queue<GPSData> gps_data_queue;
mutex queue_mutex;

// Thread function to continuously read GPS data and push it to the queue
void gps_data_thread() {
    int fd = serialOpen("/dev/serial0", 9600); 

    while (1) {
        if (serialDataAvail(fd)) {
            string gps_data;
            char ch;
            do {
                ch = serialGetchar(fd);
                gps_data += ch;
            } while (ch != '\n');

            if (gps_data.substr(0, 6) == "$GPGGA") {
                GPSData gps_data_parsed = extract_gps_data(gps_data);
                cout << "Latitude: " << gps_data_parsed.latitude << endl;
                cout << "Longitude: " << gps_data_parsed.longitude << endl;
                cout << "Time: " << gps_data_parsed.time << endl;
                
                // Lock the queue before pushing new data to it
                queue_mutex.lock();
                gps_data_queue.push(gps_data_parsed);
                queue_mutex.unlock();
            }
        }
        // Sleep for 1 millisecond to avoid excessive CPU usage
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

// Thread function to continuously read GPS data from the queue and calculate distance, speed and time
void gps_data_processing_thread() {
    GPSData gps_data1;
    GPSData gps_data2;

    while (1) {
        // Lock the queue before accessing it
        queue_mutex.lock();

        // Pop the first GPS data from the queue
        if (gps_data_queue.size() > 2) {
            cout << "gps_data1 pop!!!!!!!!!" << endl;
            gps_data1 = gps_data_queue.front();
            gps_data_queue.pop();
            
            cout << "gps_data2 pop!!!!!!!!!" << endl;
            gps_data2 = gps_data_queue.front();
            gps_data_queue.pop();
        }

        // Unlock the queue after accessing it
        queue_mutex.unlock();

        // Calculate distance, time and speed if two valid GPS data are available
        if (gps_data1.latitude != 0.0 && gps_data1.longitude != 0.0 &&
            gps_data2.latitude != 0.0 && gps_data2.longitude != 0.0) {

            double distance = calc_distance(gps_data1.latitude, gps_data1.longitude, gps_data2.latitude, gps_data2.longitude);
            double time_interval = gps_data2.time - gps_data1.time;
            double speed = distance / time_interval;

            std::cout << "Distance: " << distance << " meters" << std::endl;
            std::cout << "Time Interval: " << time_interval << " seconds" << std::endl;
            std::cout << "Speed: " << speed << " meters/second" << std::endl;
        }
        //else {
         //   std::cout << "Failed to extract GPS data from input string" << std::endl;
        //}

        // Sleep for 1 millisecond to avoid excessive CPU usage
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

int main() { 
    // Start the GPS data thread
    thread gps_thread(gps_data_thread);

    // Start the GPS data processing thread
    thread gps_processing_thread(gps_data_processing_thread);

    // Wait for the threads to finish (which they never will)
    gps_thread.join();
    gps_processing_thread.join();

    return 0;
}
