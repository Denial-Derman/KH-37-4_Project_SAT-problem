//SchedulePlanner.cpp

#include "SchedulePlanner.h"
#include "SATSolver.h"
#include <fstream>
#include <direct.h>  // Для перевірки директорії на Windows
#include <Windows.h>
#include <iostream>

using namespace std;

// Конструктор
SchedulePlanner::SchedulePlanner(
    const vector<Group>& groups, const vector<Teacher>& teachers,
    const vector<Room>& rooms, const vector<Subject>& subjects,
    const vector<Assignment>& assignments)
    : groups(groups), teachers(teachers), rooms(rooms),
      subjects(subjects), assignments(assignments) {
    // Тут ви використовуєте assignments для створення обмежень
    cnf = createScheduleSAT(subjects, teachers, groups, assignments, rooms);
    addTeacherConstraints();
    addRoomConstraints();
    addGroupConstraints();
}


// Генерація розкладу
bool SchedulePlanner::generateSchedule() {
    unordered_set<int> assignment;
    if (!DPLL(cnf, assignment)) {
        cout << "Не вдалося створити коректний розклад." << endl;
        return false;
    }

    decodeAssignment(assignment); // Декодуємо результат у розклад
    cout << "Розклад успішно створено." << endl;
    return true;
}

// Перевірка на існування директорії
bool directoryExists(const string& dirPath) {
    DWORD ftyp = GetFileAttributesA(dirPath.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) {
        return false; // Якщо директорія не існує
    }
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) {
        return true; // Якщо це директорія
    }
    return false; // Якщо це файл, а не директорія
}

void SchedulePlanner::saveSchedule(const string& filename) const {
    string directoryPath = filename.substr(0, filename.find_last_of("\\/")); // Отримуємо директорію з шляху

    // Перевірка, чи існує директорія
    if (!directoryExists(directoryPath)) {
        cerr << "Помилка: Директорія не існує. Створюємо її..." << endl;
        if (_mkdir(directoryPath.c_str()) != 0) { // Створення директорії
            cerr << "Не вдалося створити директорію." << endl;
            return;
        }
    }

    ofstream file(filename);
    if (file.is_open()) {
        file << "Розклад:\n";
        for (const auto& entry : schedule) {
            file << entry << "\n";
        }
        file.close();
        cout << "Розклад збережено у файл: " << filename << endl;
    }
    else {
        cerr << "Не вдалося відкрити файл для запису: " << filename << endl;
    }
}

// Декодування SAT-результату у розклад
void SchedulePlanner::decodeAssignment(const unordered_set<int>& assignment) {
    for (int literal : assignment) {
        if (literal > 0) {
            int subjectId = literal / 100;
            int timeslot = (literal / 10) % 10;
            int roomId = literal % 10;

            string entry = "Предмет: " + subjects[subjectId - 1].getName() +  // Використовуємо геттер
                ", Часовий слот: " + to_string(timeslot) +
                ", Аудиторія: " + rooms[roomId - 1].getName() +  // Використовуємо геттер
                ", Викладач: " + teachers[subjects[subjectId - 1].getTeacherId() - 1].getName();  // Використовуємо геттер
            schedule.push_back(entry);
        }
    }
}

// Додавання обмежень для викладачів
void SchedulePlanner::addTeacherConstraints() {
    for (const auto& teacher : teachers) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                if (subject.getTeacherId() == teacher.getId()) {  // Використовуємо геттер
                    for (int room = 1; room <= rooms.size(); ++room) {
                        clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // Використовуємо геттер
                    }
                }
            }
            cnf.addClause(clause);
        }
    }
}

// Додавання обмежень для аудиторій
void SchedulePlanner::addRoomConstraints() {
    for (int room = 1; room <= rooms.size(); ++room) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // Використовуємо геттер
            }
            cnf.addClause(clause);
        }
    }
}

// Додавання обмежень для груп
void SchedulePlanner::addGroupConstraints() {
    for (const auto& group : groups) {
        for (int timeslot = 1; timeslot <= 5; ++timeslot) {
            Clause clause;
            for (const auto& subject : subjects) {
                if (subject.getGroupId() == group.getId()) {  // Використовуємо геттер
                    for (int room = 1; room <= rooms.size(); ++room) {
                        clause.addLiteral(subject.getId() * 100 + timeslot * 10 + room);  // Використовуємо геттер
                    }
                }
            }
            cnf.addClause(clause);
        }
    }
}

// Отримання ID викладачів
vector<int> SchedulePlanner::getTeacherIds() const {
    vector<int> ids;
    for (const auto& teacher : teachers) ids.push_back(teacher.getId());  // Використовуємо геттер
    return ids;
}

// Отримання ID груп
vector<int> SchedulePlanner::getGroupIds() const {
    vector<int> ids;
    for (const auto& group : groups) ids.push_back(group.getId());  // Використовуємо геттер
    return ids;
}
