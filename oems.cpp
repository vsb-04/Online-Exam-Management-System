#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

// Forward declarations
class Exam;
class Result;
class ExamManager;
class Question;
class Student;
class Teacher;

// User Class
class User {
protected:
    string name;
    string email;
    string password;

public:
    User(const string &name, const string &email, const string &password)
        : name(name), email(email), password(password) {}

    virtual ~User() {} // Virtual destructor
    
    bool login(const string &enteredPassword) const {
        return password == enteredPassword;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }
};

// Question Class
class Question {
private:
    string questionText;
    string questionType; // Always "True/False" now
    vector<int> correctAnswers; // Only 1 or 2 (1 for True, 2 for False)

public:
    Question(const string &text, const vector<int> &correct, const string &type = "True/False")
        : questionText(text), correctAnswers(correct), questionType(type) {}

    void displayQuestion() const {
        cout << "Question: " << questionText << endl;
        cout << "1. True (t)" << endl;
        cout << "2. False (f)" << endl;
    }

    bool evaluateAnswer(int studentAnswer) const {
        return (correctAnswers[0] == studentAnswer);
    }

    string getQuestionType() const {
        return questionType;
    }
};

// Exam Class
class Exam {
private:
    string examTitle;
    int duration;
    vector<Question> questions;

public:
    Exam(const string &title, int duration) : examTitle(title), duration(duration) {}

    void addQuestion(const Question &question) {
        questions.push_back(question);
    }

    string getExamTitle() const {
        return examTitle;
    }

    const vector<Question>& getQuestions() const {
        return questions;
    }
};

// Result Class
class Result {
private:
    const User &student;
    const Exam &exam;
    int score;

public:
    Result(const User &student, const Exam &exam, int score)
        : student(student), exam(exam), score(score) {}

    void displayResult() const {
        cout << "Student: " << student.getName() << endl;
        cout << "Exam: " << exam.getExamTitle() << endl;
        cout << "Score: " << score << endl;
    }
};

// Student Class
class Student : public User {
private:
    string studentID;
    vector<Result> examResults;

public:
    Student(const string &name, const string &email, const string &password, const string &studentID)
        : User(name, email, password), studentID(studentID) {}

    void takeExam(Exam &exam) {
        cout << "Starting Exam: " << exam.getExamTitle() << endl;

        int score = 0;
        const vector<Question>& questions = exam.getQuestions();

        for (const Question &question : questions) {
            question.displayQuestion();
            char studentAnswer;

            cout << "Enter your answer (t for True, f for False): ";
            cin >> studentAnswer;

            int answer = (studentAnswer == 't' || studentAnswer == 'T') ? 1 : (studentAnswer == 'f' || studentAnswer == 'F') ? 2 : 0;

            if (answer == 0) {
                cout << "Invalid answer, skipping question." << endl;
            } else if (question.evaluateAnswer(answer)) {
                cout << "Correct!" << endl;
                score++;
            } else {
                cout << "Incorrect." << endl;
            }
        }

        Result result(*this, exam, score);
        examResults.push_back(result);

        cout << "Exam completed! Your score: " << score << "/" << questions.size() << endl;
    }

    void viewResults() const {
        cout << "Exam Results: " << endl;
        for (const Result &result : examResults) {
            result.displayResult();
            cout << "------------------------" << endl;
        }
    }
};

// Teacher Class
class Teacher : public User {
private:
    string teacherID;

public:
    Teacher(const string &name, const string &email, const string &password, const string &teacherID)
        : User(name, email, password), teacherID(teacherID) {}

    void createExam(ExamManager &examManager);
};

// ExamManager Class
class ExamManager {
private:
    vector<Exam> exams;

public:
    void addExam(const Exam &exam) {
        exams.push_back(exam);
    }

    const vector<Exam>& getExams() const {
        return exams;
    }

    Exam* findExamByTitle(const string &title) {
        for (Exam &exam : exams) {
            if (exam.getExamTitle() == title) {
                return &exam;
            }
        }
        return nullptr;
    }
};

// Implementation of createExam method for Teacher
void Teacher::createExam(ExamManager &examManager) {
    string title;
    int duration;
    cout << "Enter the exam title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter the exam duration (in minutes): ";
    cin >> duration;

    Exam exam(title, duration);

    char addMore;
    do {
        string questionText;
        cout << "Enter question text: ";
        cin.ignore();
        getline(cin, questionText);

        vector<int> correctAnswers;
        int correctAnswer;
        cout << "Enter the correct answer (1 for True, 2 for False): ";
        cin >> correctAnswer;
        correctAnswers.push_back(correctAnswer);

        Question question(questionText, correctAnswers);
        exam.addQuestion(question);

        cout << "Add another question? (y/n): ";
        cin >> addMore;
    } while (addMore == 'y');

    examManager.addExam(exam);
    cout << "Exam created successfully!" << endl;
}

// UserManager Class
class UserManager {
private:
    vector<User *> users;

public:
    void registerUser(User *user) {
        users.push_back(user);
    }

    User* loginUser(const string &email, const string &password) {
        for (User *user : users) {
            if (user->getEmail() == email && user->login(password)) {
                return user;
            }
        }
        return nullptr;
    }

    const vector<User *> &getUsers() const {
        return users;
    }
};

// UserInterface Class
class UserInterface {
public:
    void displayMenu() {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
    }

    int getUserChoice() {
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        return choice;
    }

    void registerUser(UserManager &userManager) {
        string name, email, password, userType;
        cout << "Enter name: ";
        cin >> name;
        cout << "Enter email: ";
        cin >> email;
        cout << "Enter password: ";
        cin >> password;
        cout << "Enter user type ('s' for student, 't' for teacher): ";
        cin >> userType;

        User* newUser = nullptr;
        if (userType == "s") {
            newUser = new Student(name, email, password, "S001");
        } else if (userType == "t") {
            newUser = new Teacher(name, email, password, "T001");
        }

        if (newUser != nullptr) {
            userManager.registerUser(newUser);
            cout << "User registered successfully!" << endl;
        } else {
            cout << "Invalid user type. Registration failed." << endl;
        }
    }

    void login(UserManager &userManager, ExamManager &examManager) {
        string email, password;
        cout << "Enter email: ";
        cin >> email;
        cout << "Enter password: ";
        cin >> password;

        User* user = userManager.loginUser(email, password);
        if (user != nullptr) {
            if (Student* student = dynamic_cast<Student*>(user)) {
                studentMenu(*student, examManager); // Show student-specific menu
            } else if (Teacher* teacher = dynamic_cast<Teacher*>(user)) {
                teacherMenu(*teacher, examManager); // Show teacher-specific menu
            }
        } else {
            cout << "Login Failed! Invalid email or password." << endl;
        }
    }

    void studentMenu(Student &student, ExamManager &examManager) {
        int choice;
        do {
            cout << "\nStudent Menu:" << endl;
            cout << "1. Take Exam" << endl;
            cout << "2. View Results" << endl;
            cout << "3. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    {
                        string examTitle;
                        cout << "Enter exam title to take: ";
                        cin >> examTitle;

                        Exam *exam = examManager.findExamByTitle(examTitle);
                        if (exam != nullptr) {
                            student.takeExam(*exam);
                        } else {
                            cout << "Exam not found!" << endl;
                        }
                        break;
                    }
                case 2:
                    student.viewResults();
                    break;
                case 3:
                    cout << "Logged out successfully!" << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
            }
        } while (choice != 3);
    }

    void teacherMenu(Teacher &teacher, ExamManager &examManager) {
        int choice;
        do {
            cout << "\nTeacher Menu:" << endl;
            cout << "1. Create Exam" << endl;
            cout << "2. Logout" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    teacher.createExam(examManager);
                    break;
                case 2:
                    cout << "Logged out successfully!" << endl;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
                    break;
            }
        } while (choice != 2);
    }
};

int main() {
    UserManager userManager;
    ExamManager examManager;
    UserInterface ui;

    while (true) {
        ui.displayMenu();
        int choice = ui.getUserChoice();

        switch (choice) {
            case 1:
                ui.registerUser(userManager);
                break;
            case 2:
                ui.login(userManager, examManager);
                break;
            case 3:
                cout << "Exiting..." << endl;
                exit(0);
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
