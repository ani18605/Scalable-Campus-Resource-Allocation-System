#ifndef CLASSROOM_MANAGER_H
#define CLASSROOM_MANAGER_H

#include "models.h"
#include "TimeTree.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

class ClassroomManager {
public:
    std::unordered_map<std::string, Classroom> classrooms;
    std::unordered_map<std::string, std::shared_ptr<TimeTree>> classTrees;

    ClassroomManager() = default;

    std::string addClassroom(const std::string& classId, const std::string& buildingId, int floor, int capacity);
    std::string bookClassroom(const std::string& classId, int startMin, int endMin, int seats);
    std::string bookAnyClassroom(const std::string& buildingId, int startMin, int endMin, int seats);
    std::string queryFreeSeats(const std::string& buildingId, int startMin, int endMin);
    std::string getClassroomStatus(const std::string& classId);
};

#endif // CLASSROOM_MANAGER_H
