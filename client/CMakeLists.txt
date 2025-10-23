# Client - Qt5 приложение с графическим интерфейсом
add_executable(client
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/client.h
    src/client.cpp
    src/network_client.h
    src/network_client.cpp
    src/ui/mainwindow.ui
)

# Включение директорий
target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common/include
)

# Связывание с Qt5 и общей библиотекой
target_link_libraries(client PRIVATE 
    common
    Qt5::Core
    Qt5::Widgets
)

# Автоматическая обработка MOC файлов Qt
set_target_properties(client PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Установка исполняемого файла
install(TARGETS client
    RUNTIME DESTINATION bin
)
