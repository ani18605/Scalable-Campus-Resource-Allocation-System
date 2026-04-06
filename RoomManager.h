#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "models.h"
#include "SegmentTree.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

class RoomManager {
public:
    std::unordered_map<std::string, HostelRoom> rooms;
    std::unordered_map<std::string, std::shared_ptr<SegmentTree>> blockTrees;
    std::unordered_map<std::string, std::vector<std::string>> blockRoomLists;
    std::unordered_map<std::string, std::pair<std::string, int>> roomIndex;

    RoomManager() = default;

    std::string addRoom(const std::string& roomId, const std::string& blockId, int floor, int capacity);
    
    std::shared_ptr<SegmentTree> getBlockTree(const std::string& blockId);
    HostelRoom* getRoom(const std::string& roomId);

    // Maintenance
    std::string setMaintenance(const std::string& scopeType, const std::string& scopeId, bool state);

    // Status
    std::string getRoomStatus(const std::string& roomId);
    
    // Assumes tree is built per block
    void finalizeBlock(const std::string& blockId);
};

#endif // ROOM_MANAGER_H
