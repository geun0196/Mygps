#ifndef GPS_FUNCTIONS_H
#define GPS_FUNCTIONS_H

#include <string>
#include "GPSData.h"

std::string rawGps2degGps(int type, std::string token);
GPSData extract_gps_data(const std::string& gps_str);
double calc_distance(double lat1, double lon1, double lat2, double lon2);

#endif // GPS_FUNCTIONS_H
