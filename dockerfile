# ---------- Stage 1: Build ----------
FROM gcc:12 as builder
WORKDIR /app

COPY . .
RUN g++ -std=c++17 -O2 -o quiz main.cpp

# ---------- Stage 2: Runtime ----------
FROM debian:stable-slim
WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends ca-certificates && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /data && touch /data/leaderboard.txt && ln -s /data/leaderboard.txt /app/leaderboard.txt

COPY --from=builder /app/quiz /app/quiz
COPY --from=builder /app/questions_easy.txt /app/questions_easy.txt
COPY --from=builder /app/questions_medium.txt /app/questions_medium.txt
COPY --from=builder /app/questions_hard.txt /app/questions_hard.txt

RUN chmod +x /app/quiz

ENTRYPOINT ["/app/quiz"]
