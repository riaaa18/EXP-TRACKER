#include <iostream>
#include <string>

using namespace std;

class Expense
{
private:
    float amount;
    string category;
    int week;


public:
    void input(int current_week);
    float get_amount();
    string get_category();
    int get_week();
};

class Budget
{
private:
    float monthly_salary;
    int number_of_weeks;
    Expense expenses[100];
    int expense_count;
    float weekly_total[4];

public:
    void start();
    void input_weeks();
    void input_expenses();
    void calculate_statistics();
};



void Budget::start()
{
    expense_count = 0;

    for(int i = 0; i < 4; i++)
    {
        weekly_total[i] = 0;
    }

    cout << "Enter your monthly salary: ";
    cin >> monthly_salary;

    input_weeks();
    input_expenses();
    calculate_statistics();
    
}


void Budget::input_weeks()
{
    do
    {
        cout << "Enter number of weeks (1-4): ";
        cin >> number_of_weeks;

        if (number_of_weeks < 1 || number_of_weeks > 4)
        {
            cout << "Invalid input. Please enter a number from 1 to 4.\n";
        }

    } while (number_of_weeks < 1 || number_of_weeks > 4);
}


void Expense::input(int current_week)
{
    week = current_week;

    cout << "\nExpense for Week " << week << endl;


    cout << "\nChoose a category\n";
    cout << "1. Food\n";
    cout << "2. Transport\n";
    cout << "3. Bills\n";
    cout << "4. Entertainment\n";
    cout << "5. Shopping\n";
    cout << "6. Other\n";
    cout << "7. Custom\n";

int choice;

cout << "Enter your choice: ";
cin >> choice;

switch(choice)
{
case 1:
    category = "Food";
    break;

case 2:
    category = "Transport";
    break;

case 3:
    category = "Bills";
    break;

case 4:
    category = "Entertainment";
    break;

case 5:
    category = "Shopping";
    break;

case 6:
    category = "Other";
    break;

case 7:
    cout << "Enter custom category: ";
    cin >> category;
    break;

default:
    cout << "Invalid choice! Setting category to Other.\n";
    category = "Other";
}


        do
        {
            cout << "Enter amount: ";
            cin >> amount;

            if(amount < 0)
            {
                cout << "Amount cannot be negative.\n";
            }

        } while(amount < 0);
}



void Budget::input_expenses()
{
    int number_of_expenses;
    for(int week = 1; week <= number_of_weeks; week++)
    {
        cout << "\nEnter number of expenses for Week " << week << ": ";
        cin >> number_of_expenses;
        for(int i = 0; i < number_of_expenses; i++)
        {
            cout << "\nExpense " << i + 1 << endl;
            expenses[expense_count].input(week);
            expense_count++;
        }
    }
}


float Expense::get_amount()
{
    return amount;
}

string Expense::get_category()
{
    return category;
}

int Expense::get_week()
{
    return week;
}



void Budget::calculate_statistics()
{
    float total_expenses = 0;
        for(int i = 0; i < expense_count ; i++)
    {
        total_expenses += expenses[i].get_amount();
    }
    cout << "Total expenses: $" << total_expenses << endl;

    float remaining_savings = monthly_salary - total_expenses;

    cout << "Remaining Savings: $" << remaining_savings << endl;

    float percentage_spent = (total_expenses / monthly_salary) * 100;

    cout << "Percentage of Income Spent: " << percentage_spent << "%" << endl;

    float avg_weekly_expense = total_expenses / number_of_weeks;

    cout << "Average Weekly Expense: $" << avg_weekly_expense << endl;

    float avg_monthly_expense = total_expenses / 1; 

    cout << "Average Monthly Expense: $" << avg_monthly_expense << endl;



    for(int i = 0; i < expense_count; i++)
    {
        int week = expenses[i].get_week();
        float amount = expenses[i].get_amount();

        weekly_total[week - 1] += amount;
    }
    for(int i = 0; i < number_of_weeks; i++)
    {
        cout << "Week " << i + 1
            << ": $" << weekly_total[i] << endl;
    }



    float highest_week_total = weekly_total[0];
    int highest_week = 1;

    for(int i = 1; i < number_of_weeks; i++)
        {
            if(weekly_total[i] > highest_week_total)
            {
                highest_week_total = weekly_total[i];
                highest_week = i + 1;
            }
        }

    cout << "\nHighest Spending Week: Week " << highest_week << " ($" << highest_week_total << ")" << endl;

        string category_names[100];
        float category_totals[100];
        int category_count = 0;
        bool found = false;

    for(int i = 0; i < expense_count; i++)
{
    string current_category = expenses[i].get_category();
    float current_amount = expenses[i].get_amount();

    for(int j = 0; j < category_count; j++)
{
    if(category_names[j] == current_category)
    {
        category_totals[j] += current_amount;
        found = true;
        break;
    }
}
if(!found)
{
    category_names[category_count] = current_category;
    category_totals[category_count] = current_amount;
    category_count++;
}
}

float highest_category_total = category_totals[0];
string highest_category = category_names[0];

for(int i = 1; i < category_count; i++)
{
    if(category_totals[i] > highest_category_total)
    {
        highest_category_total = category_totals[i];
        highest_category = category_names[i];
    }
}


cout << "\nHighest Spending Category: " << highest_category<< " ($" << highest_category_total << ")" << endl;

}
    

    


int main()
{
    Budget budget_tracker;
    budget_tracker.start();

    return 0;
}