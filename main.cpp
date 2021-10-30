//
//  main.cpp
//  final
//
//  Created by research on 29.12.2020.
//

/*
 */

#include <iostream>
#include <sstream>
#include <exception>
#include <set>
#include <map>
#include <iomanip>
#include <string>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
    Date()
    {
        year = 0;
        month = 0;
        day = 0;
    }
    Date(int new_year, int new_month, int new_day)
    {
        year = new_year;
        if (new_month >= 1 && new_month <= 12)
        {
            month = new_month;
        } else {
            throw out_of_range("Month value is invalid: " + to_string(new_month));
        }
        if (new_day >= 1 && new_day <= 31)
        {
            day = new_day;
        } else {
            throw out_of_range("Day value is invalid: " + to_string(new_day));
        }
    }

    int GetYear() const
    {
        return year;
    }
    int GetMonth() const
    {
        return month;
    }
    int GetDay() const
    {
        return day;
    }

private:
    int day;
    int year;
    int month;
};

Date InputDate(const string& input_date)
{
    int year, month, day;
    bool day_bool = true;
    istringstream stream_date(input_date);

    stream_date >> year;
    if (stream_date.peek() == '-')
    {
        stream_date.ignore(1);
        stream_date >> month;
        if (stream_date.peek() == '-')
        {
            stream_date.ignore(1);
            day_bool = day_bool && stream_date >> day;
            if (!day_bool)
            {
                throw logic_error("Wrong date format: " + input_date);
            }
            if (!stream_date.eof())
            {
                throw logic_error("Wrong date format: " + input_date);
            }
        } else {
            throw logic_error("Wrong date format: " + input_date);
        }
    } else {
        throw logic_error("Wrong date format: " + input_date);
    }
    return Date(year, month, day);
};

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear())
    {
        return (lhs.GetYear() < rhs.GetYear());
    } else if (lhs.GetMonth() != rhs.GetMonth()) {
        return (lhs.GetMonth() < rhs.GetMonth());
    } else {
        return (lhs.GetDay() < rhs.GetDay());
    }
}

class Database {
public:
    void AddEvent(const Date& date, const string& event)
    {
        storage_for_data[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event)
    {
        if (storage_for_data.count(date) != 0 && storage_for_data[date].count(event) != 0)
        {
            storage_for_data[date].erase(event);
            return true;
        } else {
            return false;
        }
    }
    int DeleteDate(const Date& date)
    {
        int n = 0;
        if (storage_for_data.count(date) != 0)
        {
            n = storage_for_data[date].size();
            storage_for_data.erase(date);
            return n;
        } else {
            return n;
        }
    }

    void Find(const Date& date) const
    {
        if (storage_for_data.count(date) != 0)
        {
            for (auto& item : storage_for_data.at(date))
            {
                cout << item << endl;
            }
        }

    }

  void Print() const
    {
        for (auto& date : storage_for_data)
        {
            for (auto& event : date.second)
            {
                cout << setfill('0');
                cout << setw(4) << date.first.GetYear() << "-" << setw(2) << date.first.GetMonth() << "-" << setw(2) << date.first.GetDay() << " " << event << endl;
            }
        }
    }
private:
    map<Date, set<string>> storage_for_data;

};


int main() {
    Database db;
    string command;
    try {
        while (getline(cin, command))
        {
            // Считайте команды с потока ввода и обработайте каждую
            if (command == "")
            {
                continue;
            }
            stringstream input_stream(command);
            string what_operation, date, event;
            input_stream >> what_operation >> date >> event;
            if (what_operation == "Add")
            {
                db.AddEvent(InputDate(date), event);
            } else if (what_operation == "Del" && event != "") {
                if (db.DeleteEvent(InputDate(date), event))
                {
                    cout << "Deleted successfully" << endl;
                } else {
                    cout << "Event not found" << endl;
                }
            } else if (what_operation == "Del" && event == "") {
                Date correct_date = InputDate(date);
                cout << "Deleted " << db.DeleteDate(correct_date) << " events" << endl;
            } else if (what_operation == "Find") {
                db.Find(InputDate(date));
            } else if (what_operation == "Print") {
                db.Print();
            } else {
                throw logic_error("Unknown command: " + what_operation);
            }
        }
    } catch (exception& ex) {
        cout << ex.what() << endl;
    }

  return 0;
}

/* solution
 #include <iomanip>
 #include <iostream>
 #include <map>
 #include <set>
 #include <sstream>
 #include <stdexcept>
 #include <string>
 #include <vector>

 using namespace std;

 class Date {
 public:
   // конструктор выбрасывает исключение, если его аргументы некорректны
   Date(int new_year, int new_month, int new_day) {
     year = new_year;
     if (new_month > 12 || new_month < 1) {
       throw logic_error("Month value is invalid: " + to_string(new_month));
     }
     month = new_month;
     if (new_day > 31 || new_day < 1) {
       throw logic_error("Day value is invalid: " + to_string(new_day));
     }
     day = new_day;
   }

   int GetYear() const {
     return year;
   }
   int GetMonth() const {
     return month;
   }
   int GetDay() const {
     return day;
   }

 private:
   int year;
   int month;
   int day;
 };

 // определить сравнение для дат необходимо для использования их в качестве ключей словаря
 bool operator<(const Date& lhs, const Date& rhs) {
   // воспользуемся тем фактом, что векторы уже можно сравнивать на <:
   // создадим вектор из года, месяца и дня для каждой даты и сравним их
   return vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
       vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
 }

 // даты будут по умолчанию выводиться в нужном формате
 ostream& operator<<(ostream& stream, const Date& date) {
   stream << setw(4) << setfill('0') << date.GetYear() <<
       "-" << setw(2) << setfill('0') << date.GetMonth() <<
       "-" << setw(2) << setfill('0') << date.GetDay();
   return stream;
 }

 class Database {
 public:
   void AddEvent(const Date& date, const string& event) {
     storage[date].insert(event);
   }

   bool DeleteEvent(const Date& date, const string& event) {
     if (storage.count(date) > 0 && storage[date].count(event) > 0) {
       storage[date].erase(event);
       return true;
     }
     return false;
   }

   int DeleteDate(const Date& date) {
     if (storage.count(date) == 0) {
       return 0;
     } else {
       const int event_count = storage[date].size();
       storage.erase(date);
       return event_count;
     }
   }

   set<string> Find(const Date& date) const {
     if (storage.count(date) > 0) {
       return storage.at(date);
     } else {
       return {};
     }
   }

   void Print() const {
     for (const auto& item : storage) {
       for (const string& event : item.second) {
         cout << item.first << " " << event << endl;
       }
     }
   }

 private:
   map<Date, set<string>> storage;
 };

 Date ParseDate(const string& date) {
   istringstream date_stream(date);
   bool ok = true;

   int year;
   ok = ok && (date_stream >> year);
   ok = ok && (date_stream.peek() == '-');
   date_stream.ignore(1);

   int month;
   ok = ok && (date_stream >> month);
   ok = ok && (date_stream.peek() == '-');
   date_stream.ignore(1);

   int day;
   ok = ok && (date_stream >> day);
   ok = ok && date_stream.eof();

   if (!ok) {
     throw logic_error("Wrong date format: " + date);
   }
   return Date(year, month, day);
 }

 int main() {
   try {
     Database db;

     string command_line;
     while (getline(cin, command_line)) {
       stringstream ss(command_line);

       string command;
       ss >> command;

       if (command == "Add") {

         string date_str, event;
         ss >> date_str >> event;
         const Date date = ParseDate(date_str);
         db.AddEvent(date, event);

       } else if (command == "Del") {

         string date_str;
         ss >> date_str;
         string event;
         if (!ss.eof()) {
           ss >> event;
         }
         const Date date = ParseDate(date_str);
         if (event.empty()) {
           const int count = db.DeleteDate(date);
           cout << "Deleted " << count << " events" << endl;
         } else {
           if (db.DeleteEvent(date, event)) {
             cout << "Deleted successfully" << endl;
           } else {
             cout << "Event not found" << endl;
           }
         }

       } else if (command == "Find") {

         string date_str;
         ss >> date_str;
         const Date date = ParseDate(date_str);
         for (const string& event : db.Find(date)) {
           cout << event << endl;
         }

       } else if (command == "Print") {

         db.Print();

       } else if (!command.empty()) {

         throw logic_error("Unknown command: " + command);

       }
     }
   } catch (const exception& e) {
     cout << e.what() << endl;
   }

   return 0;
 }
 */
