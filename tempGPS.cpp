#include <iostream>
#include <wiringSerial.h>

int main()
{
    int fd = serialOpen("/dev/serial0", 9600); 
    std::string set_frequency_command = "$PMTK220,100";
	serialPrintf(fd, set_frequency_command.c_str());

    while (1) {
        if (serialDataAvail(fd)) {
            std::string gps_data;
            char ch;
            do {
                ch = serialGetchar(fd);
                gps_data += ch;
            } while (ch != '\n');
            
            
            if (gps_data.substr(0, 6) == "$GPGGA") {
                std::string gpgga_data[15];
                int index = 0;
                size_t pos = 0;
                while ((pos = gps_data.find(",")) != std::string::npos && index < 15) {
                    gpgga_data[index] = gps_data.substr(0, pos);
                    gps_data.erase(0, pos + 1);
                    index++;
                }
                
                
                std::string time_utc = gpgga_data[1];
                std::string latitude = gpgga_data[2];
                std::string latitude_direction = gpgga_data[3];
                std::string longitude = gpgga_data[4];
                std::string longitude_direction = gpgga_data[5];
                std::string gps_quality = gpgga_data[6];
                std::string satellites_used = gpgga_data[7];
                std::string horizontal_dilution = gpgga_data[8];
                std::string altitude = gpgga_data[9];
                std::string altitude_unit = gpgga_data[10];
                std::string geoid_height = gpgga_data[11];
                std::string geoid_height_unit = gpgga_data[12];
                
                
                std::cout << "Time (UTC): " << time_utc << std::endl;
                std::cout << "Latitude: " << latitude << " " << latitude_direction << std::endl;
                std::cout << "Longitude: " << longitude << " " << longitude_direction << std::endl;
                std::cout << "GPS Quality: " << gps_quality << std::endl;
                std::cout << "Satellites Used: " << satellites_used << std::endl;
                std::cout << "Horizontal Dilution: " << horizontal_dilution << std::endl;
                std::cout << "Altitude: " << altitude << " " << altitude_unit << std::endl;
                std::cout << "Geoid Height: " << geoid_height << " " << geoid_height_unit << std::endl;
            }
        }
    }

    return 0;
}
