#ifndef REPORTER_H
#define REPORTER_H

#include "RoomManager.h"
#include <string>

class Reporter {
    RoomManager& roomManager;
public:
    Reporter(RoomManager& rm);
    std::string generateOccupancyReport(const std::string& scopeType, const std::string& scopeId);
};

#endif // REPORTER_H
