#include "RoomManager.h"

std::string RoomManager::addRoom(const std::string& roomId, const std::string& blockId, int floor, int capacity) {
    if (rooms.find(roomId) != rooms.end()) {
        return "ERR_ALREADY_EXISTS";
    }
    rooms[roomId] = HostelRoom(roomId, blockId, floor, capacity);
    int index = blockRoomLists[blockId].size();
    blockRoomLists[blockId].push_back(roomId);
    roomIndex[roomId] = {blockId, index};
    return "OK RoomAdded";
}

void RoomManager::finalizeBlock(const std::string& blockId) {
    int sz = blockRoomLists[blockId].size();
    blockTrees[blockId] = std::make_shared<SegmentTree>(sz);
}

std::shared_ptr<SegmentTree> RoomManager::getBlockTree(const std::string& blockId) {
    if (blockTrees.find(blockId) == blockTrees.end()) {
        if (blockRoomLists.find(blockId) != blockRoomLists.end()) {
            finalizeBlock(blockId);
        } else {
            return nullptr;
        }
    }
    return blockTrees[blockId];
}

HostelRoom* RoomManager::getRoom(const std::string& roomId) {
    auto it = rooms.find(roomId);
    if (it != rooms.end()) return &it->second;
    return nullptr;
}

std::string RoomManager::setMaintenance(const std::string& scopeType, const std::string& scopeId, bool state) {
    int count = 0;
    if (scopeType == "ROOM") {
        if (rooms.find(scopeId) == rooms.end()) return "ERR_NOT_FOUND";
        auto& room = rooms[scopeId];
        room.status = state ? RoomStatus::MAINTENANCE : (room.occupied > 0 ? RoomStatus::ALLOCATED : RoomStatus::FREE);
        auto tree = getBlockTree(room.blockId);
        int idx = roomIndex[scopeId].second;
        if (tree) tree->set_maintenance(idx, idx, state);
        count = 1;
    } else if (scopeType == "FLOOR") {
        // scopeId is something like "C 3" but input format might just give floor number, wait, PRD example: MAINTENANCE FLOOR C 3 ON -> "scopeType floor = C_3" -> we handle parsing in main.
        // Let's assume scopeId is the blockId + "_" + floor e.g. "C_3"
        // Actually PRD: "MAINTENANCE FLOOR C 3 ON". Let's assume scopeId is encoded as "C_3" for API.
        std::string blockPrefix = scopeId.substr(0, scopeId.find('_'));
        int floor = std::stoi(scopeId.substr(scopeId.find('_') + 1));
        auto tree = getBlockTree(blockPrefix);
        if (!tree) return "ERR_NOT_FOUND";
        int startIdx = -1, endIdx = -1;
        for (int i = 0; i < (int)blockRoomLists[blockPrefix].size(); ++i) {
            auto& roomId = blockRoomLists[blockPrefix][i];
            auto& room = rooms[roomId];
            if (room.floor == floor) {
                if (startIdx == -1) startIdx = i;
                endIdx = i;
                room.status = state ? RoomStatus::MAINTENANCE : (room.occupied > 0 ? RoomStatus::ALLOCATED : RoomStatus::FREE);
                count++;
            } else if (startIdx != -1) {
                // Assuming floor rooms are consecutive, if not, we do point updates
            }
        }
        // Since we aren't guaranteed floor rooms are consecutive in insertion order (though likely), point updates or lazy
        // For safe lazy, assuming floor rooms are consecutive:
        if (startIdx != -1 && endIdx != -1 && endIdx - startIdx + 1 == count) {
            tree->set_maintenance(startIdx, endIdx, state);
        } else {
            // Unordered, fallback to point updates
            for (int i = 0; i < (int)blockRoomLists[blockPrefix].size(); ++i) {
                auto& roomId = blockRoomLists[blockPrefix][i];
                if (rooms[roomId].floor == floor) tree->set_maintenance(i, i, state);
            }
        }
    } else if (scopeType == "BLOCK") {
        auto tree = getBlockTree(scopeId);
        if (!tree) return "ERR_NOT_FOUND";
        for (auto& roomId : blockRoomLists[scopeId]) {
            auto& room = rooms[roomId];
            room.status = state ? RoomStatus::MAINTENANCE : (room.occupied > 0 ? RoomStatus::ALLOCATED : RoomStatus::FREE);
            count++;
        }
        tree->set_maintenance(0, count - 1, state);
    } else {
        return "ERR_INVALID_REQUEST";
    }
    return "OK MAINTENANCE_UPDATED " + std::to_string(count);
}

std::string RoomManager::getRoomStatus(const std::string& roomId) {
    if (rooms.find(roomId) == rooms.end()) return "ERR_NOT_FOUND";
    auto& room = rooms[roomId];
    std::string s_status = "FREE";
    if (room.status == RoomStatus::ALLOCATED) s_status = "ALLOCATED";
    else if (room.status == RoomStatus::MAINTENANCE) s_status = "MAINTENANCE";
    return "ROOM " + roomId + " BLOCK " + room.blockId + " FLOOR " + std::to_string(room.floor) + 
           " CAPACITY " + std::to_string(room.capacity) + " STATUS " + s_status + " OCCUPIED " + std::to_string(room.occupied);
}
