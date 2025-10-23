# Тесты для проекта
enable_testing()

# Поиск библиотеки для тестирования
find_package(GTest REQUIRED)

# Тесты для общей библиотеки
add_executable(test_common
    test_common.cpp
    test_protocol.cpp
    test_serialization.cpp
    test_utils.cpp
)

# Включение директорий
target_include_directories(test_common PRIVATE
    ${CMAKE_SOURCE_DIR}/common/include
    ${GTEST_INCLUDE_DIRS}
)

# Связывание с библиотеками
target_link_libraries(test_common PRIVATE
    common
    ${GTEST_LIBRARIES}
    Threads::Threads
)

# Добавление тестов
add_test(NAME CommonTests COMMAND test_common)

# Установка тестов
install(TARGETS test_common
    RUNTIME DESTINATION bin
)

