# Logger - сервер логов через именованные каналы (FIFO)
add_executable(logger
    src/main.cpp
    src/logger.h
    src/logger.cpp
    src/fifo_handler.h
    src/fifo_handler.cpp
    src/log_writer.h
    src/log_writer.cpp
)

# Включение директорий
target_include_directories(logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# Связывание с общей библиотекой
target_link_libraries(logger PRIVATE common)

# Установка исполняемого файла
install(TARGETS logger
    RUNTIME DESTINATION bin
)

