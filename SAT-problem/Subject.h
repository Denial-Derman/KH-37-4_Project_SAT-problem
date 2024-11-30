#ifndef SUBJECT_H
#define SUBJECT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Subject {
private:
    int id;
    string subjectName;
    int weeklySessions;

public:
    Subject(int id, const string& name, int weeklySessions)
        : id(id), subjectName(name), weeklySessions(weeklySessions) {}

    int getId() const { return id; }
    string getName() const { return subjectName; }
    int getWeeklySessions() const { return weeklySessions; }

    static vector<Subject> loadSubjects(const string& filename) {
        vector<Subject> subjects;
        ifstream file(filename);
        int id, weeklySessions;
        string name;

        if (file.is_open()) {
            while (file >> id >> name >> weeklySessions) {
                subjects.emplace_back(id, name, weeklySessions);
            }
            file.close();
        }
        else {
            cerr << "Не вдалося відкрити файл: " << filename << endl;
        }

        return subjects;
    }
};

#endif // SUBJECT_H