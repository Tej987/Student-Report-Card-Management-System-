#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class Student {
private:
    int rollNumber;
    string name;
    double marks[5]; // Marks for 5 subjects
    double totalMarks;
    char grade;

public:
    // Constructor
    Student(int roll, string studentName, double studentMarks[]) {
        rollNumber = roll;
        name = studentName;
        totalMarks = 0;

        for (int i = 0; i < 5; i++) {
            marks[i] = studentMarks[i];
            totalMarks += marks[i];
        }
        calculateGrade();
    }

    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    double getTotalMarks() const { return totalMarks; }
    char getGrade() const { return grade; }

    // Function to calculate grade based on total marks
    void calculateGrade() {
        double percentage = (totalMarks / 500.0) * 100;
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 80) grade = 'B';
        else if (percentage >= 70) grade = 'C';
        else if (percentage >= 60) grade = 'D';
        else grade = 'F';
    }

    // Display student details
    void displayStudent() const {
        cout << "Roll No: " << rollNumber << " | Name: " << name
             << " | Total Marks: " << totalMarks << "/500"
             << " | Grade: " << grade << endl;
    }

    // Save student details to file
    void saveToFile(ofstream &outfile) const {
        outfile << rollNumber << "," << name;
        for (int i = 0; i < 5; i++) {
            outfile << "," << marks[i];
        }
        outfile << "," << totalMarks << "," << grade << endl;
    }

    // Load student details from file
    static Student loadFromFile(ifstream &infile) {
        int roll;
        string studentName;
        double studentMarks[5], total;
        char studentGrade;

        infile >> roll;
        infile.ignore();
        getline(infile, studentName, ',');
        for (int i = 0; i < 5; i++) {
            infile >> studentMarks[i];
            infile.ignore();
        }
        infile >> total >> studentGrade;

        return Student(roll, studentName, studentMarks);
    }
};

// Function to check if roll number is unique
bool isRollNumberUnique(vector<Student> &students, int roll) {
    for (const auto &s : students) {
        if (s.getRollNumber() == roll) {
            return false;
        }
    }
    return true;
}

// Function to find student by roll number
Student* findStudent(vector<Student> &students, int roll) {
    for (auto &s : students) {
        if (s.getRollNumber() == roll) {
            return &s;
        }
    }
    return nullptr;
}

// Save students to file
void saveStudentsToFile(vector<Student> &students) {
    ofstream outfile("students.txt", ios::trunc);
    for (const auto &s : students) {
        s.saveToFile(outfile);
    }
    outfile.close();
}

// Load students from file
void loadStudentsFromFile(vector<Student> &students) {
    ifstream infile("students.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int roll;
        string name;
        double marks[5], total;
        char grade;

        infile >> roll;
        infile.ignore();
        if (getline(infile, name, ',') && infile >> marks[0] >> marks[1] >> marks[2] >> marks[3] >> marks[4] >> total >> grade) {
            students.push_back(Student(roll, name, marks));
        }
    }
    infile.close();
}

// Function to delete a student record
void deleteStudent(vector<Student> &students, int roll) {
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getRollNumber() == roll) {
            students.erase(it);
            cout << "Student record deleted successfully!\n";
            saveStudentsToFile(students);
            return;
        }
    }
    cout << "Student not found!\n";
}

int main() {
    vector<Student> students;
    loadStudentsFromFile(students);

    int choice;
    while (true) {
        cout << "\n===== Student Report Card Management System =====\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student by Roll Number\n";
        cout << "4. Delete a Student\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int roll;
            string name;
            double marks[5];
            cout << "Enter Roll Number: ";
            cin >> roll;
            cin.ignore();

            if (!isRollNumberUnique(students, roll)) {
                cout << "Error! Roll Number already exists.\n";
                continue;
            }

            cout << "Enter Student Name: ";
            getline(cin, name);
            for (int i = 0; i < 5; i++) {
                cout << "Enter Marks for Subject " << (i + 1) << ": ";
                cin >> marks[i];
            }

            students.push_back(Student(roll, name, marks));
            saveStudentsToFile(students);
            cout << "Student added successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== List of Students =====\n";
            for (const auto &s : students) {
                s.displayStudent();
            }

        } else if (choice == 3) {
            int roll;
            cout << "Enter Roll Number to search: ";
            cin >> roll;
            Student* student = findStudent(students, roll);

            if (student) {
                student->displayStudent();
            } else {
                cout << "Student not found!\n";
            }

        } else if (choice == 4) {
            int roll;
            cout << "Enter Roll Number to delete: ";
            cin >> roll;
            deleteStudent(students, roll);

        } else if (choice == 5) {
            cout << "Exiting Student Report Card System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
