//Вітка тестова Завдов'єва Дениса 

#include <iostream>
#include "SchedulePlanner.h"
#include "Assignment.h"
#include <string>
#include <Windows.h> 

using namespace std;

int main() {
    SetConsoleCP(1251);

    cout << "Це наш перший сумісний проєкт! І так, почнімо" << endl;

    // Завантажуємо дані
    vector<Group> groups = Group::loadGroups("groups.txt");
    if (groups.empty()) {
        cerr << "Помилка: Не вдалося завантажити групи." << endl;
        return 1;
    }

    vector<Teacher> teachers = Teacher::loadTeachers("teachers.txt");
    if (teachers.empty()) {
        cerr << "Помилка: Не вдалося завантажити викладачів." << endl;
        return 1;
    }

    vector<Room> rooms = Room::loadRooms("rooms.txt");
    if (rooms.empty()) {
        cerr << "Помилка: Не вдалося завантажити аудиторії." << endl;
        return 1;
    }

    vector<Subject> subjects = Subject::loadSubjects("subjects.txt");
    if (subjects.empty()) {
        cerr << "Помилка: Не вдалося завантажити предмети." << endl;
        return 1;
    }

    // Зчитування асоціацій
    vector<Assignment> assignments = Assignment::loadAssignments("assignments.txt");
    if (assignments.empty()) {
        cerr << "Помилка: Не вдалося завантажити асоціації." << endl;
        return 1;
    }

    // Створюємо об'єкт планувальника
    SchedulePlanner planner(groups, teachers, rooms, subjects, assignments);

    // Генеруємо розклад
    if (planner.generateSchedule()) {
        string filename;
        cout << "Введіть назву файлу для збереження розкладу (можна вказати шлях): ";
        cin.ignore(); // Очищаємо вхід
        getline(cin, filename); // Вводимо шлях/назву файлу

        // Перевіряємо, чи вказано розширення .txt
        if (filename.find(".txt") == string::npos) {
            filename += ".txt"; // Додаємо розширення, якщо відсутнє
        }

        // Зберігаємо розклад   
        planner.saveSchedule(filename);
    } else {
        cout << "Не вдалося створити розклад." << endl;
    }

    return 0;
}
