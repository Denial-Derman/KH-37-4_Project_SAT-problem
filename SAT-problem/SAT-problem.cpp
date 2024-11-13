//Вітка тестова Завдов'єва Дениса 

#include <iostream>
#include <vector>
#include <unordered_set>
#include "CNF.h"
#include <Windows.h> 

using namespace std;

CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups);
bool DPLL(const CNF& formula, unordered_set<int>& assignment);
void addTeacherConflicts(CNF& formula, const vector<int>& teachers);
void addRoomConflicts(CNF& formula, int numSubjects, int numTimeslots, int numRooms);
void addGroupConflicts(CNF& formula, const vector<int>& groups, int numTimeslots, int numRooms);

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Це наш перший сумісний проєкт! І так, почнімо" << endl;
    // Приклад ініціалізації параметрів
    int numSubjects = 3;
    int numTimeslots = 5;
    int numRooms = 2;
    vector<int> teachers = { 1, 2 }; // Список викладачів
    vector<int> groups = { 1, 2, 3 }; // Список груп

    // Створення SAT формули
    CNF formula = createScheduleSAT(numSubjects, numTimeslots, numRooms, teachers, groups);

    // Вивід формули на консоль
    formula.print();

    // Виклик DPLL для розв'язання
    unordered_set<int> assignment;
    if (DPLL(formula, assignment)) {
        cout << "Формула задовольняється." << endl;
    }
    else {
        cout << "Формула не має розв'язку." << endl;
    }

    return 0;
}

CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups) {
    CNF formula;

    // Додаємо базові умови
    for (int subject = 1; subject <= numSubjects; ++subject) {
        Clause clause;
        for (int timeslot = 1; timeslot <= numTimeslots; ++timeslot) {
            for (int room = 1; room <= numRooms; ++room) {
                clause.addLiteral(subject * 100 + timeslot * 10 + room);
            }
        }
        formula.addClause(clause);
    }

    // Додаємо конфлікти
    addTeacherConflicts(formula, teachers);
    addRoomConflicts(formula, numSubjects, numTimeslots, numRooms);
    addGroupConflicts(formula, groups, numTimeslots, numRooms);

    return formula;
}

bool DPLL(const CNF& formula, unordered_set<int>& assignment) {
    bool unitPropagationOccurred = true;

    while (unitPropagationOccurred) {
        unitPropagationOccurred = false;
        for (const auto& clause : formula.clauses) {
            int unassignedCount = 0;
            int lastLiteral = 0;
            for (int literal : clause.literals) {
                if (assignment.find(literal) == assignment.end() && assignment.find(-literal) == assignment.end()) {
                    unassignedCount++;
                    lastLiteral = literal;
                }
            }
            if (unassignedCount == 0) continue;
            if (unassignedCount == 1) {
                assignment.insert(lastLiteral);
                unitPropagationOccurred = true;
            }
        }
    }

    for (const auto& clause : formula.clauses) {
        bool clauseSatisfied = false;
        for (int literal : clause.literals) {
            if (assignment.find(literal) != assignment.end()) {
                clauseSatisfied = true;
                break;
            }
        }
        if (!clauseSatisfied && !clause.isEmpty()) {
            return false;
        }
    }

    if (formula.clauses.empty()) return true;

    int chosenLiteral = formula.clauses[0].literals[0];

    assignment.insert(chosenLiteral);
    if (DPLL(formula, assignment)) return true;

    assignment.erase(chosenLiteral);
    assignment.insert(-chosenLiteral);
    if (DPLL(formula, assignment)) return true;

    return false;
}

void addTeacherConflicts(CNF& formula, const vector<int>& teachers) {
    // Приклад реалізації конфліктів викладачів
    for (size_t i = 0; i < teachers.size(); ++i) {
        for (size_t j = i + 1; j < teachers.size(); ++j) {
            Clause clause;
            // Додаємо літерали для заборони викладання в один і той же час
            // Наприклад, teachers[i] * 1000 + timeslot забороняє викладання
            // Для спрощення будемо уявляти, що викладачі викладають один предмет
            clause.addLiteral(-teachers[i]);
            clause.addLiteral(-teachers[j]);
            formula.addClause(clause);
        }
    }
}

void addRoomConflicts(CNF& formula, int numSubjects, int numTimeslots, int numRooms) {
    for (int timeslot = 1; timeslot <= numTimeslots; ++timeslot) {
        for (int room = 1; room <= numRooms; ++room) {
            Clause clause;
            for (int subject1 = 1; subject1 <= numSubjects; ++subject1) {
                for (int subject2 = subject1 + 1; subject2 <= numSubjects; ++subject2) {
                    clause.addLiteral(-(subject1 * 100 + timeslot * 10 + room));
                    clause.addLiteral(-(subject2 * 100 + timeslot * 10 + room));
                }
            }
            formula.addClause(clause);
        }
    }
}

void addGroupConflicts(CNF& formula, const vector<int>& groups, int numTimeslots, int numRooms) {
    for (int group : groups) {
        for (int timeslot = 1; timeslot <= numTimeslots; ++timeslot) {
            Clause clause;
            for (int subject = 1; subject <= groups.size(); ++subject) {
                clause.addLiteral(-(subject * 100 + timeslot * 10 + group));
            }
            formula.addClause(clause);
        }
    }
}
