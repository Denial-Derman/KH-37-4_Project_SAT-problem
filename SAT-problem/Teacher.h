#ifndef TEACHER_H
#define TEACHER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Teacher {
private:
    int id;
    string teacherName;

public:
    Teacher(int id, const string& name) : id(id), teacherName(name) {}

    int getId() const { return id; }
    string getName() const { return teacherName; }

    static vector<Teacher> loadTeachers(const string& filename) {
        vector<Teacher> teachers;
        ifstream file(filename);
        string name;
        int id;
        if (file.is_open()) {
            while (file >> id >> name) {
                teachers.emplace_back(id, name);
            }
            file.close();
        }
        else {
            cerr << "Не вдалося відкрити файл: " << filename << endl;
        }
        return teachers;
    }
};

#endif // TEACHER_H
