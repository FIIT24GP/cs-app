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
    make server1

# Создание директории для логов
RUN mkdir -p /app/logs

# Открытие порта
EXPOSE 8081

# Запуск сервера
CMD ["./build/server1/server1"]

