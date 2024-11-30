#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Group {
private:
    int id;
    string groupName;

public:
    Group(int id, const string& name) : id(id), groupName(name) {}

    int getId() const { return id; }
    string getName() const { return groupName; }

    static vector<Group> loadGroups(const string& filename) {
        vector<Group> groups;
        ifstream file(filename);
        string name;
        int id;
        if (file.is_open()) {
            while (file >> id >> name) {
                groups.emplace_back(id, name);
            }
            file.close();
        }
        else {
            cerr << "Не вдалося відкрити файл: " << filename << endl;
        }
        return groups;
    }
};

#endif // GROUP_H
