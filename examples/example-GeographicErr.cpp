// Example of using the GeographicLib::GeographicErr class

#include <GeographicLib/Constants.hpp>
#include <iostream>

using namespace std;
using namespace GeographicLib;

int main() {
    try {
        throw GeographicErr("Test throwing an exception");
    } catch (const GeographicErr& e) {
        cout << "Caught exception: " << e.what() << "\n";
    }
}
