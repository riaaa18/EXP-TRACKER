#include "InputValidation.h"
#include <iostream>
using namespace std;

int main() {
    double salary = getSalary();
    int months = getMonths();
    int totalWeeks = calculateWeeks(months);
    double weeklySalary = calculateWeeklySalary(salary);

    double weeklyTotal[MAX_WEEKS];
    initializeWeeklyTotals(weeklyTotal, totalWeeks);

    cout << "\n Summary" << endl;
    cout << "Monthly salary: Rs. " << salary << endl;
    cout << "Months to track: " << months << endl;
    cout << "Total weeks: " << totalWeeks << endl;
    cout << "Weekly budget: Rs. " << weeklySalary << endl;
    cout << "Initialized " << totalWeeks << " weeks of data (all set to 0)." << endl;

    return 0;
}
