#include "SATSolver.h"
#include <unordered_set>
#include <iostream>

using namespace std;

// Створення CNF для розкладу
CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups) {
    CNF formula;

    // Додаємо базові умови (для кожного предмета в кожному часі та аудиторії)
    for (int subject = 1; subject <= numSubjects; ++subject) {
        Clause clause;
        for (int timeslot = 1; timeslot <= numTimeslots; ++timeslot) {
            for (int room = 1; room <= numRooms; ++room) {
                clause.addLiteral(subject * 100 + timeslot * 10 + room);
            }
        }
        formula.addClause(clause);
    }

    // Тут можна додавати додаткові конфлікти, наприклад, для викладачів, груп і т.д.

    return formula;
}

// DPLL алгоритм для розв'язання SAT-проблеми
bool DPLL(const CNF& formula, unordered_set<int>& assignment) {
    bool unitPropagationOccurred = true;

    // Виконуємо одиничну пропагацію
    while (unitPropagationOccurred) {
        unitPropagationOccurred = false;
        unitPropagation(formula, assignment); // Виконуємо одиничну пропагацію
    }

    // Перевірка чи всі клаузи задоволені
    if (isSatisfiable(formula, assignment)) {
        return true;
    }

    // Якщо є незадоволені клаузи, пробуємо вибір літерала
    int literal = chooseLiteral(formula, assignment);
    if (literal == 0) return false;

    // Пробуємо додати літерал
    assignment.insert(literal);
    if (DPLL(formula, assignment)) return true;

    // Пробуємо додати заперечення літерала
    assignment.erase(literal);
    assignment.insert(-literal);
    if (DPLL(formula, assignment)) return true;

    return false;
}

// Одинична пропагація
bool unitPropagation(const CNF& formula, unordered_set<int>& assignment) {
    bool unitPropagationOccurred = false;

    // Проходимо через всі клаузи
    for (auto& clause : formula.clauses) {
        int unassignedCount = 0;
        int lastLiteral = 0;

        // Перевіряємо кожен літерал у клаузі
        for (int literal : clause.literals) {
            if (assignment.find(literal) == assignment.end() && assignment.find(-literal) == assignment.end()) {
                unassignedCount++;
                lastLiteral = literal;
            }
        }

        // Якщо є лише один невизначений літерал
        if (unassignedCount == 1) {
            assignment.insert(lastLiteral);
            unitPropagationOccurred = true;
        }
    }

    return unitPropagationOccurred;
}

// Вибір літерала для додавання до розв'язку
int chooseLiteral(const CNF& formula, const unordered_set<int>& assignment) {
    for (const auto& clause : formula.clauses) {
        for (int literal : clause.literals) {
            if (assignment.find(literal) == assignment.end() && assignment.find(-literal) == assignment.end()) {
                return literal; // Повертаємо перший невизначений літерал
            }
        }
    }
    return 0; // Якщо всі літерали визначені
}

// Перевірка чи всі клаузи задоволені
bool isSatisfiable(const CNF& formula, const unordered_set<int>& assignment) {
    for (const auto& clause : formula.clauses) {
        bool clauseSatisfied = false;

        for (int literal : clause.literals) {
            if (assignment.find(literal) != assignment.end()) {
                clauseSatisfied = true;
                break;
            }
        }

        if (!clauseSatisfied && !clause.isEmpty()) {
            return false; // Якщо хоча б одна клауза не задоволена, то формула не є виконуваною
        }
    }

    return true; // Якщо всі клаузи задоволені
}
