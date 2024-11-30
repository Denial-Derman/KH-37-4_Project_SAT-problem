//SchedulePlanner.cpp

#include "SchedulePlanner.h"
#include "SATSolver.h"
#include <fstream>
#include <direct.h>  // ��� �������� �������� �� Windows
#include <Windows.h>
#include <iostream>

using namespace std;

// �����������
SchedulePlanner::SchedulePlanner(
    const vector<Group>& groups, const vector<Teacher>& teachers,
    const vector<Room>& rooms, const vector<Subject>& subjects,
    const vector<Assignment>& assignments)
    : groups(groups), teachers(teachers), rooms(rooms),
      subjects(subjects), assignments(assignments) {
    // ��� �� ������������� assignments ��� ��������� ��������
    cnf = createScheduleSAT(subjects, teachers, groups, assignments, rooms);
    addTeacherConstraints();
    addRoomConstraints();
    addGroupConstraints();
}


// ��������� ��������
bool SchedulePlanner::generateSchedule() {
    unordered_set<int> assignment;
    if (!DPLL(cnf, assignment)) {
        cout << "�� ������� �������� ��������� �������." << endl;
        return false;
    }

    decodeAssignment(assignment); // �������� ��������� � �������
    cout << "������� ������ ��������." << endl;
    return true;
}

// �������� �� ��������� ��������
bool directoryExists(const string& dirPath) {
    DWORD ftyp = GetFileAttributesA(dirPath.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return false; // ���� ��������� �� ����
    }
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        return true; // ���� �� ���������
    }
    return false; // ���� �� ����, � �� ���������
}

void SchedulePlanner::saveSchedule(const string& filename) const {
    string directoryPath = filename.substr(0, filename.find_last_of("\\/")); // �������� ��������� � �����

    // ��������, �� ���� ���������
    if (!directoryExists(directoryPath)) {
        cerr << "�������: ��������� �� ����. ��������� ��..." << endl;
        if (_mkdir(directoryPath.c_str()) != 0) { // ��������� ��������
            cerr << "�� ������� �������� ���������." << endl;
            return;
        }
    }

    ofstream file(filename);
    if (file.is_open()) {
        file << "�������:\n";
        for (const auto& entry : schedule) {
            file << entry << "\n";
        }
        file.close();
        cout << "������� ��������� � ����: " << filename << endl;
    }
    else {
        cerr << "�� ������� ������� ���� ��� ������: " << filename << endl;
    }
}

// ����������� SAT-���������� � �������
void SchedulePlanner::decodeAssignment(const unordered_set<int>& assignment) {
    for (int literal : assignment) {
        if (literal > 0) {
            int subjectId = literal / 100;
            int timeslot = (literal / 10) % 10;
            int roomId = literal % 10;

            string entry = "�������: " + subjects[subjectId - 1].getName() +  // ������������� ������
                ", ������� ����: " + to_string(timeslot) +
                ", ��������: " + rooms[roomId - 1].getName() +  // ������������� ������
                ", ��������: " + teachers[subjects[subjectId - 1].getTeacherId() - 1].getName();  // ������������� ������
            schedule.push_back(entry);
        }
    }
}

// ��������� �������� ��� ����������
void SchedulePlanner::addTeacherConstraints() {
    for (const auto& teacher : teachers) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                if (subject.getTeacherId() == teacher.getId()) {  // ������������� ������
                    for (int room = 1; room <= rooms.size(); ++room) {
                        clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // ������������� ������
                    }
                }
            }
            cnf.addClause(clause);
        }
    }
}

// ��������� �������� ��� ��������
void SchedulePlanner::addRoomConstraints() {
    for (int room = 1; room <= rooms.size(); ++room) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // ������������� ������
            }
            cnf.addClause(clause);
        }
    }
}

// ��������� �������� ��� ����
void SchedulePlanner::addGroupConstraints() {
    for (const auto& group : groups) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                if (subject.getGroupId() == group.getId()) {  // ������������� ������
                    for (int room = 1; room <= rooms.size(); ++room) {
                        clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // ������������� ������
                    }
                }
            }
            cnf.addClause(clause);
        }
    }
}

// ��������� ID ����������
vector<int> SchedulePlanner::getTeacherIds() const {
    vector<int> ids;
    for (const auto& teacher : teachers) ids.push_back(teacher.getId());  // ������������� ������
    return ids;
}

// ��������� ID ����
vector<int> SchedulePlanner::getGroupIds() const {
    vector<int> ids;
    for (const auto& group : groups) ids.push_back(group.getId());  // ������������� ������
    return ids;
}
