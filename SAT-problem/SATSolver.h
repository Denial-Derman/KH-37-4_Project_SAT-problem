#ifndef SATSOLVER_H
#define SATSOLVER_H

#include <unordered_set>
#include "CNF.h"
#include "Clause.h"

using namespace std;

// ������� ��� ��������� CNF ������� ��� ��������
CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups);

// DPLL �������� ��� ����'������ SAT-��������
bool DPLL(const CNF& formula, unordered_set<int>& assignment);

// �������� ��� ������ ���������� ������� � ���������� ����
bool unitPropagation(const CNF& formula, unordered_set<int>& assignment);

// ������� ��� ������ ������� ��� ��������� �� ����'����
int chooseLiteral(const CNF& formula, const unordered_set<int>& assignment);

// ������� ��� �������� �� �� ������ ���������
bool isSatisfiable(const CNF& formula, const unordered_set<int>& assignment);

#endif // SATSOLVER_H
