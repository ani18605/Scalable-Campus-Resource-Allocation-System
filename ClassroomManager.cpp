#include "ClassroomManager.h"

std::string ClassroomManager::addClassroom(const std::string& classId, const std::string& buildingId, int floor, int capacity) {
    if (classrooms.find(classId) != classrooms.end()) {
        return "ERR_ALREADY_EXISTS";
    }
    classrooms[classId] = Classroom(classId, buildingId, floor, capacity);
    classTrees[classId] = std::make_shared<TimeTree>();
    return "OK ClassroomAdded";
}

std::string ClassroomManager::bookClassroom(const std::string& classId, int startMin, int endMin, int seats) {
    if (classrooms.find(classId) == classrooms.end()) return "ERR_NOT_FOUND";
    auto& c = classrooms[classId];
    if (c.status == ClassroomStatus::MAINTENANCE) return "ERR_NOT_AVAILABLE";
    
    bool ok = classTrees[classId]->book_seats(startMin, endMin, seats, c.capacity);
    if (!ok) return "NOT_ENOUGH_SEATS";
    return "BOOKED";
}

std::string ClassroomManager::bookAnyClassroom(const std::string& buildingId, int startMin, int endMin, int seats) {
    for (auto& [id, c] : classrooms) {
        if (c.buildingId == buildingId && c.status == ClassroomStatus::ACTIVE && c.capacity >= seats) {
            int max_used = classTrees[id]->max_seats_used(startMin, endMin);
            if (max_used == 0) { // Don't assign same classroom single time
                classTrees[id]->book_seats(startMin, endMin, c.capacity, c.capacity); // Fully reserve
                return "BOOKED 🏫 " + id;
            }
        }
    }
    return "NOT_ENOUGH_SEATS 🚫";
}

std::string ClassroomManager::queryFreeSeats(const std::string& buildingId, int startMin, int endMin) {
    int total_free = 0;
    for (auto& [id, c] : classrooms) {
        if (c.buildingId == buildingId && c.status == ClassroomStatus::ACTIVE) {
            int max_used = classTrees[id]->max_seats_used(startMin, endMin);
            total_free += std::max(0, c.capacity - max_used);
        }
    }
    return "FREE_SEATS " + std::to_string(total_free);
}

std::string ClassroomManager::getClassroomStatus(const std::string& classId) {
    if (classrooms.find(classId) == classrooms.end()) return "ERR_NOT_FOUND";
    auto& c = classrooms[classId];
    std::string s_status = (c.status == ClassroomStatus::ACTIVE) ? "ACTIVE" : "MAINTENANCE";
    return "CLASSROOM " + classId + " BUILDING " + c.buildingId + " FLOOR " + std::to_string(c.floor) + 
           " CAPACITY " + std::to_string(c.capacity) + " STATUS " + s_status;
}
