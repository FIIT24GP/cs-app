FROM ubuntu:20.04

# Установка зависимостей
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    qt5-default \
    qtbase5-dev \
    qtbase5-dev-tools \
    libqt5widgets5 \
    libqt5core5a \
    libqt5gui5 \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Создание рабочей директории
WORKDIR /app

# Копирование исходного кода
COPY . .

# Сборка проекта
RUN mkdir build && cd build && \
    cmake .. && \
    make client

# Открытие порта
EXPOSE 8080

# Запуск клиента
CMD ["./build/client/client"]
