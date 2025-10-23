# Server1 - получение информации о жёстких дисках, файловой системе и раскладке клавиатуры
add_executable(server1
    src/main.cpp
    src/server1.h
    src/server1.cpp
    src/disk_info.h
    src/disk_info.cpp
    src/filesystem_info.h
    src/filesystem_info.cpp
    src/keyboard_info.h
    src/keyboard_info.cpp
)

# Включение директорий
target_include_directories(server1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# Связывание с общей библиотекой
target_link_libraries(server1 PRIVATE common)

# Установка исполняемого файла
install(TARGETS server1
    RUNTIME DESTINATION bin
)
