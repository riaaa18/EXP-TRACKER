#include "InputValidation.h"
#include <iostream>
using namespace std;

double getSalary() {
    double salary;
    cout << "Enter your monthly salary: Rs. ";
    cin >> salary;

    while (salary <= 0) {
        cout << "Salary must be greater than 0. Try again: ";
        cin >> salary;
    }

    return salary;
}

int getMonths() {
    int months;
    cout << "Enter number of months you want to track: ";
    cin >> months;

    while (months <= 0) {
        cout << "Months must be greater than 0. Try again: ";
        cin >> months;
    }

    return months;
}

int calculateWeeks(int months) {
    return months * 4;
}

double calculateWeeklySalary(double salary) {
    return salary / 4;
}

void initializeWeeklyTotals(double weeklyTotal[], int totalWeeks) {
    for (int i = 0; i < totalWeeks; i++) {
        weeklyTotal[i] = 0;
    }
}
