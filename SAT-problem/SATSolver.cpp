#include "SATSolver.h"
#include <unordered_set>
#include <iostream>

using namespace std;

// ��������� CNF ��� ��������
CNF createScheduleSAT(int numSubjects, int numTimeslots, int numRooms, const vector<int>& teachers, const vector<int>& groups) {
    CNF formula;

    // ������ ����� ����� (��� ������� �������� � ������� ��� �� �������)
    for (int subject = 1; subject <= numSubjects; ++subject) {
        Clause clause;
        for (int timeslot = 1; timeslot <= numTimeslots; ++timeslot) {
            for (int room = 1; room <= numRooms; ++room) {
                clause.addLiteral(subject * 100 + timeslot * 10 + room);
            }
        }
        formula.addClause(clause);
    }

    // ��� ����� �������� �������� ��������, ���������, ��� ����������, ���� � �.�.

    return formula;
}

// DPLL �������� ��� ����'������ SAT-��������
bool DPLL(const CNF& formula, unordered_set<int>& assignment) {
    bool unitPropagationOccurred = true;

    // �������� �������� ����������
    while (unitPropagationOccurred) {
        unitPropagationOccurred = false;
        unitPropagation(formula, assignment); // �������� �������� ����������
    }

    // �������� �� �� ������ ���������
    if (isSatisfiable(formula, assignment)) {
        return true;
    }

    // ���� � ����������� ������, ������� ���� �������
    int literal = chooseLiteral(formula, assignment);
    if (literal == 0) return false;

    // ������� ������ ������
    assignment.insert(literal);
    if (DPLL(formula, assignment)) return true;

    // ������� ������ ����������� �������
    assignment.erase(literal);
    assignment.insert(-literal);
    if (DPLL(formula, assignment)) return true;

    return false;
}

// �������� ����������
bool unitPropagation(const CNF& formula, unordered_set<int>& assignment) {
    bool unitPropagationOccurred = false;

    // ��������� ����� �� ������
    for (auto& clause : formula.clauses) {
        int unassignedCount = 0;
        int lastLiteral = 0;

        // ���������� ����� ������ � �����
        for (int literal : clause.literals) {
            if (assignment.find(literal) == assignment.end() && assignment.find(-literal) == assignment.end()) {
                unassignedCount++;
                lastLiteral = literal;
            }
        }

        // ���� � ���� ���� ������������ ������
        if (unassignedCount == 1) {
            assignment.insert(lastLiteral);
            unitPropagationOccurred = true;
        }
    }

    return unitPropagationOccurred;
}

// ���� ������� ��� ��������� �� ����'����
int chooseLiteral(const CNF& formula, const unordered_set<int>& assignment) {
    for (const auto& clause : formula.clauses) {
        for (int literal : clause.literals) {
            if (assignment.find(literal) == assignment.end() && assignment.find(-literal) == assignment.end()) {
                return literal; // ��������� ������ ������������ ������
            }
        }
    }
    return 0; // ���� �� ������� ��������
}

// �������� �� �� ������ ���������
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
            return false; // ���� ���� � ���� ������ �� ����������, �� ������� �� � �����������
        }
    }

    return true; // ���� �� ������ ���������
}
