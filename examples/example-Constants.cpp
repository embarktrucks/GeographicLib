// Example of using the GeographicLib::Constants class

#include <GeographicLib/Constants.hpp>
#include <exception>
#include <iostream>

using namespace std;
using namespace GeographicLib;

int main() {
    try {
        cout << "WGS84 parameters:\n"
             << "a = " << Constants::WGS84_a() << " m\n"
             << "f = 1/" << 1 / Constants::WGS84_f() << "\n";
    } catch (const exception& e) {
        cerr << "Caught exception: " << e.what() << "\n";
        return 1;
    }
}
