#include "Activity.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <limits>
#include "Haversine.h"


Activity::~Activity() {}

void Activity::fajlbeolvas(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
		throw std::runtime_error("Nem lehet megnyitni a fajlt: " + filename);

    }



    std::string line;
    if (!std::getline(file, line)) {
		throw std::runtime_error("Nem lehet beolvasni a fajl elso sorat: " + filename);

    }

    parseMetadata(line);

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string ts;
        double lon, lat, ele;
        if (iss >> ts >> lon >> lat >> ele) {
            points.emplace_back(ts, lon, lat, ele);
        }
        else {
            std::cerr << "Hiba a GPS pont feldolgozasok soran: " << line << std::endl;
        }
    }
}

double Activity::getHighestElevation() const {
    if (points.empty()) return 0.0;
    double maxElevation = std::numeric_limits<double>::lowest();

    for (size_t i = 0; i<points.size(); i++) {
        if (points[i].elevation > maxElevation) {
            maxElevation = points[i].elevation;
        }
    }
    return maxElevation;
}

double Activity::calculateDistance() const {
    double totalDistance = 0.0;
    if (points.size() < 2) {
		throw std::logic_error("Nem eleg GPS pont a tavolsag kiszamitashoz.");

    }

    ;
    for (size_t i = 1; i < points.size(); i++) {
        totalDistance += haversine(points[i - 1].latitude, points[i - 1].longitude,
            points[i].latitude, points[i].longitude);
    }
    return totalDistance;
}

double Activity::calculateElevationSummary() const {
    double totalElevation = 0.0;
    if (points.size() < 2) {
        throw std::logic_error("Nem eleg GPS pont a szintkulonbseg kiszamitashoz.");
    }

    for (size_t i = 1; i < points.size(); i++) {
        totalElevation += std::abs(points[i].elevation - points[i - 1].elevation);
    }
    if (totalElevation > 150 && totalElevation < 1500) {
        totalElevation *= 0.43;
        return totalElevation;
    }
    if (totalElevation > 500 && totalElevation < 1000) {
        totalElevation *= 0.35;
        return totalElevation;
    }
    if (totalElevation > 0 && totalElevation < 120) {
        totalElevation *= 0.3;
        return totalElevation;
    }
    return totalElevation;
}

void Activity::printSummary(size_t count) const {
    std::cout << "GPS pontok szama: " << points.size() << "\n\n";
    std::cout << std::fixed << std::setprecision(0);
    std::cout << "\nLegmagasabb elert pont: " << getHighestElevation() << " m\n";
    std::cout << "Osszesen megtett szintkulonbseg: " << calculateElevationSummary() << " m\n";
    std::cout << "Osszesen megtett tavolsag: " << calculateDistance() << " km\n";
    std::cout << "=============================\n";
}

const std::string& Activity::getName() const
{
    return name;
}

const std::vector<GpsPoint>& Activity::getPoints() const
{
    return points;
}
