#pragma once
#ifndef ACTIVITY_H
#define ACTIVITY_H
#include <iostream>
#include <string>
#include <vector>
#include "GpsPoint.h"

class Activity {
protected:
    std::string name;
    std::vector<GpsPoint> points;
public:
    virtual ~Activity();
    virtual void fajlbeolvas(const std::string& filename);
    virtual void parseMetadata(const std::string& line) = 0;
    virtual void printSummary(size_t count = 5) const;

   
    const std::string& getName() const;
    const std::vector<GpsPoint>& getPoints() const;

    double getHighestElevation() const;
    double calculateDistance() const;
    double calculateElevationSummary() const;
};

#endif 


