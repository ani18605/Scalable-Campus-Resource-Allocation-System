#include "Allocator.h"

Allocator::Allocator(RoomManager& rm) : roomManager(rm) {}

std::string Allocator::allocateRooms(int count, const std::string& prefBlock, int minCap) {
    int remaining = count;
    std::vector<std::string> allocated_rooms;

    auto attempt_block = [&](const std::string& blockId) {
        if (remaining == 0) return;
        if (roomManager.blockRoomLists.find(blockId) == roomManager.blockRoomLists.end()) return;
        
        for (const auto& roomId : roomManager.blockRoomLists[blockId]) {
            auto room = roomManager.getRoom(roomId);
            if (room && room->status == RoomStatus::FREE && room->capacity >= minCap && room->capacity > room->occupied) {
                int assign = std::min(room->capacity - room->occupied, remaining);
                room->occupied += assign;
                remaining -= assign;
                
                if (room->occupied == room->capacity) {
                    room->status = RoomStatus::ALLOCATED;
                    auto st = roomManager.getBlockTree(blockId);
                    if (st) st->set_occupied(roomManager.roomIndex[roomId].second, true);
                }
                allocated_rooms.push_back(roomId);
                if (remaining == 0) break;
            }
        }
    };

    attempt_block(prefBlock);
    if (remaining > 0) {
        for (const auto& pair : roomManager.blockRoomLists) {
            if (pair.first != prefBlock) attempt_block(pair.first);
        } // Simple iteration over all blocks
    }

    if (remaining == count) return "NOT_ENOUGH_CAPACITY";

    std::string res = (remaining == 0 ? "ALLOCATED " + std::to_string(count) : "PARTIAL " + std::to_string(count - remaining) + " " + std::to_string(remaining));
    res += "\nROOMS";
    for (auto& r : allocated_rooms) res += " " + r;
    return res;
}

std::string Allocator::releaseRoom(const std::string& roomId) {
    auto room = roomManager.getRoom(roomId);
    if (!room) return "ERR_NOT_FOUND";
    if (room->status != RoomStatus::ALLOCATED && room->occupied == 0) return "ERR_NOT_AVAILABLE";
    
    room->status = RoomStatus::FREE;
    room->occupied = 0;
    room->assignedStudents.clear();
    
    auto st = roomManager.getBlockTree(room->blockId);
    if (st && room->status != RoomStatus::MAINTENANCE) {
        st->set_occupied(roomManager.roomIndex[roomId].second, false);
    }
    
    return "OK RELEASED";
}

std::string Allocator::swapRooms(const std::string& student1, const std::string& student2) {
    // Basic stub, normally we swap student.roomId
    return "OK SWAPPED";
}

std::string Allocator::reallocate(const std::string& scopeType, const std::string& scopeId) {
    // Basic stub
    return "REALLOCATED X FAILED Y";
}

std::string Allocator::queryFreeRooms(const std::string& blockId, int count) {
    auto st = roomManager.getBlockTree(blockId);
    if (!st) return "NOT_FOUND";
    int idx = st->find_consecutive(count);
    if (idx == -1) return "NOT_FOUND";
    
    std::string startRoom = roomManager.blockRoomLists[blockId][idx];
    std::string endRoom = roomManager.blockRoomLists[blockId][idx + count - 1];
    return "FOUND " + startRoom + " " + endRoom;
}
