#include "Reporter.h"
#include <algorithm>

Reporter::Reporter(RoomManager& rm) : roomManager(rm) {}

std::string Reporter::generateOccupancyReport(const std::string& scopeType, const std::string& scopeId) {
    int total = 0;
    int free = 0;
    int allocated = 0;
    int maintenance = 0;

    auto process_room = [&](const HostelRoom& room) {
        total++;
        if (room.status == RoomStatus::FREE) free++;
        else if (room.status == RoomStatus::ALLOCATED) allocated++;
        else if (room.status == RoomStatus::MAINTENANCE) maintenance++;
    };

    if (scopeType == "BLOCK") {
        if (roomManager.blockRoomLists.find(scopeId) != roomManager.blockRoomLists.end()) {
            for (auto& rId : roomManager.blockRoomLists[scopeId]) process_room(roomManager.rooms[rId]);
        } else {
            return "THIS DONESNT EXISTS";
        }
    } else if (scopeType == "CAMPUS" && scopeId == "ALL") {
        for (auto& [id, room] : roomManager.rooms) {
            process_room(room);
        }
    }

    std::string res = "TOTAL_ROOMS " + std::to_string(total) + " FREE " + std::to_string(free) + 
                      " ALLOCATED " + std::to_string(allocated) + " MAINTENANCE " + std::to_string(maintenance);
    return res;
}
