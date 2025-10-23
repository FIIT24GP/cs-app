cmake_minimum_required(VERSION 3.16)
project(cs-app VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройка компилятора
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Поиск пакетов
find_package(Threads REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

# Включение поддиректорий
add_subdirectory(common)
add_subdirectory(server1)
add_subdirectory(server2)
add_subdirectory(logger)
add_subdirectory(client)

# Включение тестов
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# Установка целей
install(TARGETS server1 server2 logger client
    RUNTIME DESTINATION bin
)

# Установка конфигурационных файлов
install(DIRECTORY deploy/ DESTINATION deploy)
