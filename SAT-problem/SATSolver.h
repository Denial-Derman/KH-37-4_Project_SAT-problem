#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <unordered_set>
#include "CNF.h"
#include "Clause.h"

using namespace std;

// Функція для створення CNF формули для розкладу
CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups);

// DPLL алгоритм для розв'язання SAT-проблеми
bool DPLL(const CNF& formula, unordered_set<int>& assignment);

// Операція для пошуку одиничного літерала і виключення його
bool unitPropagation(const CNF& formula, unordered_set<int>& assignment);

// Функція для вибору літерала для додавання до розв'язку
int chooseLiteral(const CNF& formula, const unordered_set<int>& assignment);

// Функція для перевірки чи всі клаузи задоволені
bool isSatisfiable(const CNF& formula, const unordered_set<int>& assignment);

#endif // SATSOLVER_H
