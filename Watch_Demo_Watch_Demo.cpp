#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>
using namespace std;

int Check_Leap_Year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool checkDate(string date) {
    regex date_regex(R"(\b\d{2}\.\d{2}\.\d{4}\b)");

    if (regex_match(date, date_regex)) {
        vector<string> parts;
        stringstream ss(date);
        string part;

        while (getline(ss, part, '.'))
            parts.push_back(part);

        if (parts[1] == "02" && Check_Leap_Year(stoi(parts[2])))
            if (stoi(parts[0]) > 29)
                return false;

        if (stoi(parts[0]) > 31 || stoi(parts[1]) > 12)
            return false;
    }

    else
        return false;
}

struct Record {
    string date;
    string text;
    int income;
};


int main() {
    setlocale(LC_ALL, "Ru");

    ifstream infile("Test.txt");
    if (!infile) {
        cerr << "Не удалось открыть файл!" << endl;
        return 1;
    }

    vector<Record> records;
    string line;

    regex date_regex(R"(\b\d+\.\d+\.\d+\b)");
    regex number_regex(R"(\b\d+\b)");
    regex text_regex(R"((["'])?([а-яА-Яa-zA-Z]+)(["'])?)");

    while (getline(infile, line)) {
        smatch date_match;
        if (regex_search(line, date_match, date_regex)) {
            string date = date_match[0];

            string line_without_date = regex_replace(line, date_regex, "");

            smatch number_match;
            if (regex_search(line_without_date, number_match, number_regex)) {
                string number = number_match[0];

                string without_nums = regex_replace(line_without_date, number_regex, "");

                smatch text_match;
                if (regex_search(without_nums, text_match, text_regex)) {
                    string text = text_match[2];

                    Record rec;
                    rec.date = date;
                    rec.text = text;
                    rec.income = stoi(number);
                    records.push_back(rec);
                }
            }
        }
    }

    bool Date_Mistake = false;
    for (auto& rec : records) {
        if (!checkDate(rec.date))
            Date_Mistake = true;
    }

    if (Date_Mistake)
        cout << "Ошибка! Некорректная дата в файле!" << endl;
    else {
        int choice, vibor;
        cout << "Выберите опцию\n\n1.Добавление новой строки\n2.Вывод имеющихся данных" << endl;
        cout << "Введите ваш выбор: ";
        cin >> vibor;
        cout << endl;

        if (vibor == 1) {
            string new_date, new_text, new_income_str;
            int new_income;

            // Ввод и проверка даты
            while (true) {
                cout << "Введите дату в формате ДД.ММ.ГГГГ: ";
                cin >> new_date;

                vector<string> parts;
                stringstream ss(new_date);
                string part;

                while (getline(ss, part, '.'))
                    parts.push_back(part);

                if (stoi(parts[0]) > 31 || stoi(parts[1]) > 12)
                    cout << "Ошибка: неверный формат даты! Попробуйте снова." << endl;

                else if (regex_match(new_date, date_regex)) {
                    break;
                }
                else {
                    cout << "Ошибка: неверный формат даты! Попробуйте снова." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            // Ввод и проверка суммы
            while (true) {
                cout << "Введите сумму (целое число): ";
                cin >> new_income_str;

                if (regex_match(new_income_str, number_regex)) {
                    try {
                        new_income = stoi(new_income_str);
                        break;
                    }
                    catch (const exception& e) {
                        cout << "Ошибка: неверный формат числа! Попробуйте снова." << endl;
                    }
                }
                else {
                    cout << "Ошибка: сумма должна быть целым числом! Попробуйте снова." << endl;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            // Ввод и проверка источника
            while (true) {
                cout << "Введите источник дохода: ";
                cin >> new_text;

                if (regex_match(new_text, text_regex)) {
                    break;
                }
                else {
                    cout << "Ошибка: источник должен содержать только буквы! Попробуйте снова." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }

            // Добавление в файл
            ofstream outfile("Test.txt", ios::app);
            if (!outfile) {
                cerr << "Не удалось открыть файл для записи!" << endl;
                return 1;
            }

            outfile << endl;
            outfile << new_date << " " << new_income << " \"" << new_text << "\"" << endl;
            outfile.close();

            cout << "Данные успешно добавлены в файл!" << endl;
        }
        else if (vibor == 2) {
            cout << "Надо ли отсортировать по убыванию дохода?\n\n";
            cout << "1: Да" << endl << "2: Нет" << endl << "Введите ваш выбор: ";
            cin >> choice;
            cout << "\n";

            if (choice != 1 && choice != 2) {
                cout << "Ошибка!" << endl;
            }
            else if (choice == 2) {
                cout << "Неотсортированные данные" << "\n\n";
                for (const auto& r : records) {
                    cout << "Дата: " << r.date << " | Сумма: " << r.income << " | Источник: " << r.text << endl;
                }
            }
            else if (choice == 1) {
                sort(records.begin(), records.end(), [](const Record& a, const Record& b) {
                    return a.income > b.income;
                    });
                cout << "Отсортированные данные" << "\n\n";
                for (const auto& r : records) {
                    cout << "Дата: " << r.date << " | Сумма: " << r.income << " | Источник: " << r.text << endl;
                }
            }
        }
        else {
            cout << "Некорректный выбор!" << endl;
        }
    }
    return 0;
}