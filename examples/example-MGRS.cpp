// Example of using the GeographicLib::MGRS class

#include <GeographicLib/MGRS.hpp>
#include <GeographicLib/UTMUPS.hpp>
#include <exception>
#include <iostream>
#include <string>

using namespace std;
using namespace GeographicLib;

int main() {
    try {
        // See also example-GeoCoords.cpp
        {
            // Sample forward calculation
            double lat = 33.3, lon = 44.4;  // Baghdad
            int zone;
            bool northp;
            double x, y;
            UTMUPS::Forward(lat, lon, zone, northp, x, y);
            string mgrs;
            MGRS::Forward(zone, northp, x, y, lat, 5, mgrs);
            cout << mgrs << "\n";
        }
        {
            // Sample reverse calculation
            string mgrs = "38SMB4488";
            int zone, prec;
            bool northp;
            double x, y;
            MGRS::Reverse(mgrs, zone, northp, x, y, prec);
            double lat, lon;
            UTMUPS::Reverse(zone, northp, x, y, lat, lon);
            cout << prec << " " << lat << " " << lon << "\n";
        }
    } catch (const exception& e) {
        cerr << "Caught exception: " << e.what() << "\n";
        return 1;
    }
}
