// SchedulePlanner.h
#ifndef SCHEDULE_PLANNER_H
#define SCHEDULE_PLANNER_H

#include <vector>
#include <unordered_set>
#include <string>
#include "Group.h"
#include "Teacher.h"
#include "Room.h"
#include "Subject.h"
#include "Assignment.h"
#include "CNF.h"

using namespace std;

class SchedulePlanner {
private:
    vector<Group> groups;
    vector<Teacher> teachers;
    vector<Room> rooms;
    vector<Subject> subjects;
    vector<Assignment> assignments;
    CNF cnf;
    vector<string> schedule;

    void addTeacherConstraints();
    void addRoomConstraints();
    void addGroupConstraints();
    void decodeAssignment(const unordered_set<int>& assignment);
public:
    SchedulePlanner(const vector<Group>& groups, const vector<Teacher>& teachers,
        const vector<Room>& rooms, const vector<Subject>& subjects,
        const vector<Assignment>& assignments);


    bool generateSchedule();  // Метод для генерації розкладу
    void saveSchedule(const string& filename) const; // Метод для збереження розкладу

    vector<int> getTeacherIds() const;
    vector<int> getGroupIds() const;
};

#endif // SCHEDULE_PLANNER_H