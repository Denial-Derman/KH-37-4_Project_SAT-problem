#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Room {
private:
    int id;
    string roomName;
    int capacity;

public:
    Room(int id, const string& name, int capacity) : id(id), roomName(name), capacity(capacity) {}

    int getId() const { return id; }
    string getName() const { return roomName; }
    int getCapacity() const { return capacity; }

    static vector<Room> loadRooms(const string& filename) {
        vector<Room> rooms;
        ifstream file(filename);
        string name;
        int id, capacity;
        if (file.is_open()) {
            while (file >> id >> name >> capacity) {
                rooms.emplace_back(id, name, capacity);
            }
            file.close();
        }
        else {
            cerr << "Не вдалося відкрити файл: " << filename << endl;
        }
        return rooms;
    }
};

#endif // ROOM_H
