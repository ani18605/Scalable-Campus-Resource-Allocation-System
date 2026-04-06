#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <unordered_set>

enum class RoomStatus {
    FREE,
    ALLOCATED,
    MAINTENANCE
};

struct HostelRoom {
    std::string roomId;
    std::string blockId;
    int floor;
    int capacity;
    int occupied;
    RoomStatus status;
    std::unordered_set<std::string> assignedStudents;

    HostelRoom() : floor(0), capacity(0), occupied(0), status(RoomStatus::FREE) {}
    HostelRoom(std::string rId, std::string bId, int f, int c)
        : roomId(rId), blockId(bId), floor(f), capacity(c), occupied(0), status(RoomStatus::FREE) {}
};

enum class ClassroomStatus {
    ACTIVE,
    MAINTENANCE
};

struct Classroom {
    std::string classId;
    std::string buildingId;
    int floor;
    int capacity;
    ClassroomStatus status;

    Classroom() : floor(0), capacity(0), status(ClassroomStatus::ACTIVE) {}
    Classroom(std::string cId, std::string bId, int f, int c)
        : classId(cId), buildingId(bId), floor(f), capacity(c), status(ClassroomStatus::ACTIVE) {}
};

struct Student {
    std::string studentId;
    std::string roomId;
    std::string blockId;
    int floor;
    bool isAssigned;

    Student() : floor(0), isAssigned(false) {}
    Student(std::string sId) : studentId(sId), floor(0), isAssigned(false) {}
};

#endif // MODELS_H
