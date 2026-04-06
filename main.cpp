#include <iostream>
#include <string>
#include <sstream>
#include "RoomManager.h"
#include "ClassroomManager.h"
#include "Allocator.h"
#include "Reporter.h"

int main() {
    RoomManager roomManager;
    ClassroomManager classroomManager;
    Allocator allocator(roomManager);
    Reporter reporter(roomManager);

    std::string line;
    if (!std::getline(std::cin, line)) return 0; // Skip or parse count

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string command;
        ss >> command;

        if (command == "ADD_ROOM") {
            std::string rId, bId;
            int floor, cap;
            ss >> rId >> bId >> floor >> cap;
            std::cout << roomManager.addRoom(rId, bId, floor, cap) << std::endl;
        }
        else if (command == "ADD_CLASSROOM") {
            std::string cId, bId;
            int floor, cap;
            ss >> cId >> bId >> floor >> cap;
            std::cout << classroomManager.addClassroom(cId, bId, floor, cap) << std::endl;
        }
        else if (command == "QUERY_FREE_ROOMS") {
            std::string bId;
            int count;
            ss >> bId >> count;
            std::cout << allocator.queryFreeRooms(bId, count) << std::endl;
        }
        else if (command == "ALLOCATE_ROOMS") {
            int count, minCap;
            std::string prefBlock;
            ss >> count >> prefBlock >> minCap;
            std::cout << allocator.allocateRooms(count, prefBlock, minCap) << std::endl;
        }
        else if (command == "ROOM_STATUS") {
            std::string rId;
            ss >> rId;
            std::cout << roomManager.getRoomStatus(rId) << std::endl;
        }
        else if (command == "MAINTENANCE") {
            std::string scopeType, scopeId, stateStr;
            ss >> scopeType;
            if (scopeType == "FLOOR") {
                std::string block;
                int floorNum;
                ss >> block >> floorNum >> stateStr;
                scopeId = block + "_" + std::to_string(floorNum);
            } else {
                ss >> scopeId >> stateStr;
            }
            bool state = (stateStr == "ON");
            std::cout << roomManager.setMaintenance(scopeType, scopeId, state) << std::endl;
        }
        else if (command == "REPORT") {
            std::string scopeType, scopeId;
            ss >> scopeType;
            if (scopeType == "FLOOR") {
                std::string block;
                int f;
                ss >> block >> f;
                scopeId = block + "_" + std::to_string(f);
            } else {
                ss >> scopeId;
            }
            std::cout << reporter.generateOccupancyReport(scopeType, scopeId) << std::endl;
        }
        else if (command == "BOOK_CLASSROOM") {
            std::string cId, startStr, endStr;
            int seats;
            ss >> cId >> startStr >> endStr >> seats;
            auto parseTime = [](const std::string& t) {
                return std::stoi(t.substr(0, 2)) * 60 + std::stoi(t.substr(3, 2));
            };
            std::cout << classroomManager.bookClassroom(cId, parseTime(startStr), parseTime(endStr), seats) << std::endl;
        }
        else if (command == "BOOK_ANY_CLASSROOM") {
            std::string buildingId, startStr, endStr;
            int seats;
            ss >> buildingId >> startStr >> endStr >> seats;
            auto parseTime = [](const std::string& t) {
                return std::stoi(t.substr(0, 2)) * 60 + std::stoi(t.substr(3, 2));
            };
            std::cout << classroomManager.bookAnyClassroom(buildingId, parseTime(startStr), parseTime(endStr), seats) << std::endl;
        }
        else if (command == "QUERY_FREE_SEATS") {
            std::string startStr, endStr, bId;
            ss >> startStr >> endStr >> bId;
            auto parseTime = [](const std::string& t) {
                return std::stoi(t.substr(0, 2)) * 60 + std::stoi(t.substr(3, 2));
            };
            std::cout << classroomManager.queryFreeSeats(bId, parseTime(startStr), parseTime(endStr)) << std::endl;
        }
        else if (command == "RELEASE_ROOM") {
            std::string rId;
            ss >> rId;
            std::cout << allocator.releaseRoom(rId) << std::endl;
        } else {
            std::cout << "ERR_INVALID_COMMAND" << std::endl;
        }
        
        // IPC delimiting string
        std::cout << "===END===" << std::endl;
    }

    return 0;
}
