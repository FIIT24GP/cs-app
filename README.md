# CS-App - Клиент-серверное приложение

Курсовая работа по разработке клиент-серверного приложения на C++17 с использованием TCP-сокетов и многопоточности.

## Структура проекта

```
cs-app/
├── CMakeLists.txt          # Основной CMake файл
├── common/                 # Общая библиотека
│   ├── include/           # Заголовочные файлы
│   └── src/               # Исходные файлы
├── server1/               # Сервер 1 - информация о дисках, ФС, клавиатуре
├── server2/               # Сервер 2 - информация о процессах и потоках
├── logger/                # Сервер логов
├── client/                # Qt5 клиент с GUI
├── tests/                 # Тесты
└── deploy/                # Docker конфигурация
    └── docker/
        ├── server1/
        ├── server2/
        ├── logger/
        └── client/
```

## Требования

- Ubuntu 20.04+
- GCC 9+ или Clang 10+
- CMake 3.16+
- Qt5 (для клиента)
- Docker и Docker Compose (для контейнеризации)

## Установка зависимостей

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake qt5-default qtbase5-dev qtbase5-dev-tools libqt5widgets5 libqt5core5a libqt5gui5 libboost-all-dev libgtest-dev
```

## Сборка проекта

### Локальная сборка

```bash
cd cs-app
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Сборка с тестами

```bash
cd cs-app
mkdir build
cd build
cmake -DBUILD_TESTS=ON ..
make -j$(nproc)
```

## Запуск приложения

### Локальный запуск

1. Запуск логгера:
```bash
./build/logger/logger
```

2. Запуск серверов (в отдельных терминалах):
```bash
./build/server1/server1
./build/server2/server2
```

3. Запуск клиента:
```bash
./build/client/client
```

### Запуск через Docker

```bash
cd cs-app/deploy
docker-compose up --build
```

## Архитектура

### Server1
- **Порт**: 8081
- **Функции**:
  - Получение информации о жёстких дисках
  - Получение информации о файловой системе
  - Получение информации о раскладке клавиатуры

### Server2
- **Порт**: 8082
- **Функции**:
  - Получение идентификатора серверного процесса
  - Получение списка потоков процесса

### Logger
- **Функции**:
  - Приём сообщений от серверов через именованные каналы (FIFO)
  - Запись логов в отдельные файлы

### Client
- **Интерфейс**: Qt5 Widgets
- **Функции**:
  - Подключение к обоим серверам
  - Отправка запросов и получение ответов
  - Отображение информации в графическом интерфейсе

## Протокол обмена данными

Приложение использует собственный протокол на основе TCP с бинарной сериализацией:

- **Заголовок сообщения**: 32 байта
- **Типы сообщений**: REQUEST, RESPONSE, LOG, ERROR
- **Команды**: GET_DISK_INFO, GET_FILESYSTEM_INFO, GET_KEYBOARD_INFO, GET_PROCESS_INFO, GET_THREAD_INFO

## Тестирование

```bash
cd build
make test
# или
./test_common
```

## Разработка

### Добавление новых команд

1. Добавьте новую команду в `common/include/protocol.h`
2. Реализуйте обработку в соответствующих серверах
3. Добавьте поддержку в клиенте
4. Обновите тесты

### Добавление новых серверов

1. Создайте новую директорию в корне проекта
2. Добавьте CMakeLists.txt для нового сервера
3. Обновите основной CMakeLists.txt
4. Добавьте Dockerfile в deploy/docker/

## Лицензия

Проект создан в учебных целях для курсовой работы.