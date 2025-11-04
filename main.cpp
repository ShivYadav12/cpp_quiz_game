#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <chrono>
using namespace std;
using namespace chrono;

// 🎨 ANSI color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

struct Question {
    string text;
    string optionA, optionB, optionC, optionD;
    char correctAnswer;
};

struct Player {
    string name;
    int score;
};

vector<Question> loadQuestions(const string &filename) {
    vector<Question> questions;
    ifstream file(filename);
    if (!file) {
        cerr << RED << "Error: Cannot open " << filename << "!" << RESET << endl;
        return questions;
    }

    Question q;
    while (getline(file, q.text)) {
        getline(file, q.optionA);
        getline(file, q.optionB);
        getline(file, q.optionC);
        getline(file, q.optionD);
        file >> q.correctAnswer;
        file.ignore();
        questions.push_back(q);
    }
    file.close();
    return questions;
}

void saveToLeaderboard(const string &name, int score) {
    ofstream out("leaderboard.txt", ios::app);
    if (out) {
        out << name << " " << score << endl;
    }
    out.close();
}

vector<Player> loadLeaderboard() {
    vector<Player> players;
    ifstream in("leaderboard.txt");
    Player p;
    while (in >> p.name >> p.score) {
        players.push_back(p);
    }
    in.close();

    sort(players.begin(), players.end(), [](const Player &a, const Player &b) {
        return a.score > b.score;
    });

    if (players.size() > 5)
        players.resize(5);
    return players;
}

void showLeaderboard() {
    vector<Player> players = loadLeaderboard();
    cout << "\n" << BOLD << BLUE << "===== 🏆 LEADERBOARD 🏆 =====" << RESET << "\n";
    if (players.empty()) {
        cout << YELLOW << "No scores yet. Play the game to set a record!" << RESET << endl;
        return;
    }
    for (size_t i = 0; i < players.size(); ++i) {
        cout << CYAN << i + 1 << ". " << players[i].name << " - "
             << players[i].score << " pts" << RESET << endl;
    }
}

void playQuiz(vector<Question> &questions, const string &level, const string &playerName) {
    int score = 0;
    char answer;
    const int timeLimit = 10; // seconds per question

    random_shuffle(questions.begin(), questions.end());

    cout << "\n" << BOLD << BLUE << "===== Starting " << level << " Quiz =====" << RESET << "\n";
    cout << YELLOW << "⏱️  You have " << timeLimit << " seconds per question." << RESET << "\n";

    for (size_t i = 0; i < questions.size(); ++i) {
        cout << "\n" << BOLD << "Q" << i + 1 << ": " << questions[i].text << RESET << endl;
        cout << questions[i].optionA << endl;
        cout << questions[i].optionB << endl;
        cout << questions[i].optionC << endl;
        cout << questions[i].optionD << endl;

        cout << CYAN << "Your answer (A/B/C/D): " << RESET;
        auto start = steady_clock::now();
        cin >> answer;
        auto end = steady_clock::now();

        double timeTaken = duration_cast<seconds>(end - start).count();
        answer = toupper(answer);

        if (timeTaken > timeLimit) {
            cout << RED << "⏰ Time's up! You took " << timeTaken << " seconds." << RESET << endl;
            cout << RED << "❌ No points for this question." << RESET << endl;
            continue;
        }

        if (answer == questions[i].correctAnswer) {
            cout << GREEN << "✅ Correct! (" << timeTaken << "s)" << RESET << endl;
            score++;
        } else {
            cout << RED << "❌ Wrong! Correct answer: " << questions[i].correctAnswer << RESET << endl;
        }
    }

    cout << "\n" << BOLD << YELLOW << "🎯 You scored " << score << " / " << questions.size() << RESET << endl;
    float percent = (float)score / questions.size() * 100;
    cout << "Percentage: " << percent << "%" << endl;

    saveToLeaderboard(playerName, score);

    cout << GREEN << "Your score has been saved to the leaderboard!" << RESET << endl;
}

int main() {
    srand(time(0));
    int choice;

    do {
        cout << "\n" << BOLD << BLUE << "===== QUIZ GAME MENU =====" << RESET << "\n";
        cout << "1. Easy Level\n";
        cout << "2. Medium Level\n";
        cout << "3. Hard Level\n";
        cout << "4. View Leaderboard\n";
        cout << "5. Exit\n";
        cout << CYAN << "Enter your choice: " << RESET;
        cin >> choice;

        string filename, level;

        switch (choice) {
            case 1:
                filename = "questions_easy.txt";
                level = "Easy";
                break;
            case 2:
                filename = "questions_medium.txt";
                level = "Medium";
                break;
            case 3:
                filename = "questions_hard.txt";
                level = "Hard";
                break;
            case 4:
                showLeaderboard();
                continue;
            case 5:
                cout << GREEN << "👋 Thanks for playing! See you again!" << RESET << endl;
                return 0;
            default:
                cout << RED << "Invalid choice! Try again." << RESET << endl;
                continue;
        }

        vector<Question> questions = loadQuestions(filename);
        if (questions.empty()) {
            cout << RED << "No questions available for this level." << RESET << endl;
            continue;
        }

        string playerName;
        cout << "\n" << CYAN << "Enter your name: " << RESET;
        cin >> playerName;

        playQuiz(questions, level, playerName);

        char again;
        cout << "\n" << YELLOW << "Play another quiz? (Y/N): " << RESET;
        cin >> again;
        if (toupper(again) == 'N') break;

    } while (true);

    cout << GREEN << "\nGoodbye! 👋" << RESET << endl;
    return 0;
}
