#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "RoomManager.h"

class Allocator {
    RoomManager& roomManager;
public:
    std::unordered_map<std::string, Student> students;

    Allocator(RoomManager& rm);

    std::string allocateRooms(int count, const std::string& prefBlock, int minCap);
    std::string releaseRoom(const std::string& roomId);
    std::string swapRooms(const std::string& student1, const std::string& student2);
    std::string reallocate(const std::string& scopeType, const std::string& scopeId);
    std::string queryFreeRooms(const std::string& blockId, int count);
};

#endif // ALLOCATOR_H
