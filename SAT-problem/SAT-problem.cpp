#include <iostream>
#include <vector>
#include <unordered_set>
#include "CNF.h"

using namespace std;

CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups);
bool DPLL(const CNF& formula, unordered_set<int>& assignment);
void addTeacherConflicts(CNF& formula, const vector<int>& teachers);
void addRoomConflicts(CNF& formula, int numSubjects, int numTimeslots, int numRooms);
void addGroupConflicts(CNF& formula, const vector<int>& groups, int numTimeslots, int numRooms);

int main() {
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
