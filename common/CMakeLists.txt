# Общая библиотека для всех модулей
add_library(common STATIC
    src/protocol.h
    src/protocol.cpp
    src/serialization.h
    src/serialization.cpp
    src/utils.h
    src/utils.cpp
    src/network.h
    src/network.cpp
)

# Включение директорий
target_include_directories(common PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Связывание с потоками
target_link_libraries(common PUBLIC Threads::Threads)

# Установка библиотеки
install(TARGETS common
    EXPORT commonTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Установка заголовочных файлов
install(DIRECTORY include/ DESTINATION include)

