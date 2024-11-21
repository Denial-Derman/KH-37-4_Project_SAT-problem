#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Assignment {
private:
    int subjectId;
    int teacherId;
    int groupId;

public:
    Assignment(int subjectId, int teacherId, int groupId)
        : subjectId(subjectId), teacherId(teacherId), groupId(groupId) {}

    int getSubjectId() const { return subjectId; }
    int getTeacherId() const { return teacherId; }
    int getGroupId() const { return groupId; }

    static vector<Assignment> loadAssignments(const string& filename) {
        vector<Assignment> assignments;
        ifstream file(filename);
        int subjectId, teacherId, groupId;

        if (file.is_open()) {
            while (file >> subjectId >> teacherId >> groupId) {
                assignments.emplace_back(subjectId, teacherId, groupId);
            }
            file.close();
        }
        else {
            cerr << "Не вдалося відкрити файл: " << filename << endl;
        }

        return assignments;
    }
};
#endif // ASSIGNMENT_H