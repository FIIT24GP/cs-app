# Server2 - возврат идентификатора серверного процесса и списка его потоков
add_executable(server2
    src/main.cpp
    src/server2.h
    src/server2.cpp
    src/process_info.h
    src/process_info.cpp
    src/thread_info.h
    src/thread_info.cpp
)

# Включение директорий
target_include_directories(server2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# Связывание с общей библиотекой
target_link_libraries(server2 PRIVATE common)

# Установка исполняемого файла
install(TARGETS server2
    RUNTIME DESTINATION bin
)

