#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

const int MAX_WEEKS = 500;   // big enough for any realistic number of months

double getSalary();
int getMonths();
int calculateWeeks(int months);
double calculateWeeklySalary(double salary);
void initializeWeeklyTotals(double weeklyTotal[], int totalWeeks);

#endif
