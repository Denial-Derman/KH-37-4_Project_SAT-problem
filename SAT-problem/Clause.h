// Clause.h
#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
#include <iostream>

using namespace std;

// Клас для представлення клаузи
class Clause {
public:
    vector<int> literals; // Вектор літералів

    // Додає літерал до клаузи
    void addLiteral(int literal) {
        literals.push_back(literal);
    }

    // Функція для виведення клаузи на екран
    void print() const {
        cout << "(";
        for (size_t i = 0; i < literals.size(); ++i) {
            cout << literals[i];
            if (i < literals.size() - 1) cout << " ∨ ";
        }
        cout << ")";
    }
};

#endif // CLAUSE_H

