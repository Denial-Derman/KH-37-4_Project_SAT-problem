﻿#ifndef CNF_H
#define CNF_H

#include <vector>
#include "Clause.h"

using namespace std;

// Клас для представлення CNF формули
class CNF {
public:
    vector<Clause> clauses; // Вектор клауз

    // Додає клаузу до CNF формули
    void addClause(const Clause& clause) {
        clauses.push_back(clause);
    }

    // Функція для виведення CNF формули на екран
    /*void print() const {
        for (size_t i = 0; i < clauses.size(); ++i) {
            clauses[i].print();
            if (i < clauses.size() - 1) {
                cout << " ∧ ";
            }
        }
        cout << endl;
    }*/
};

#endif // CNF_H
