A fun and interactive console-based quiz game built in C++ with features like a timer, colored output, and a persistent leaderboard.
The project also includes a Dockerfile for easy building and running in containers.

🧠Features
✅ Multiple difficulty levels — Easy, Medium, Hard
✅ Timed questions (10 seconds per question)
✅ Leaderboard with top scores (persistent across runs)
✅ Colorful console output for better user experience
✅ Docker support for easy setup and deployment

🏗️ How It Works
The game loads questions from
text files:
questions_easy.txt
questions_medium.txt
questions_hard.txt
You choose a difficulty level, enter your name, and answer questions within a time limit.
Your score and name are saved in a leaderboard.txt file.
The leaderboard shows the top 5 players.


🐳 Run with Docker
Build Image
docker build -t cpp-quiz-game .

Run the Game (Interactive)
docker run --rm -it cpp-quiz-game
