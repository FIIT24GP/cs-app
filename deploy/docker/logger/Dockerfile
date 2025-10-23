FROM ubuntu:20.04

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Создание рабочей директории
WORKDIR /app

# Копирование исходного кода
COPY . .

# Сборка проекта
RUN mkdir build && cd build && \
    cmake .. && \
    make logger

# Создание директории для логов
RUN mkdir -p /app/logs

# Создание именованных каналов
RUN mkfifo /app/logs/server1.log.fifo /app/logs/server2.log.fifo

# Запуск логгера
CMD ["./build/logger/logger"]
