#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget_(nullptr)
    , mainLayout_(nullptr)
    , connectionGroup_(nullptr)
    , connectionLayout_(nullptr)
    , connectButton_(nullptr)
    , disconnectButton_(nullptr)
    , connectionStatus_(nullptr)
    , server1Group_(nullptr)
    , server1Layout_(nullptr)
    , diskInfoButton_(nullptr)
    , filesystemInfoButton_(nullptr)
    , keyboardInfoButton_(nullptr)
    , server1Output_(nullptr)
    , server2Group_(nullptr)
    , server2Layout_(nullptr)
    , processInfoButton_(nullptr)
    , threadInfoButton_(nullptr)
    , server2Output_(nullptr)
    , statusBar_(nullptr)
    , progressBar_(nullptr)
    , statusTimer_(nullptr)
    , client_(std::make_unique<Client>())
{
    setupUI();
    setupConnections();
    
    // Запуск таймера для обновления статуса
    statusTimer_ = new QTimer(this);
    connect(statusTimer_, &QTimer::timeout, this, &MainWindow::updateStatus);
    statusTimer_->start(1000); // Обновление каждую секунду
}

MainWindow::~MainWindow() {
    if (client_) {
        client_->disconnect();
    }
}

void MainWindow::setupUI() {
    centralWidget_ = new QWidget(this);
    setCentralWidget(centralWidget_);
    
    mainLayout_ = new QVBoxLayout(centralWidget_);
    
    // Группа подключения
    connectionGroup_ = new QGroupBox("Подключение к серверам", this);
    connectionLayout_ = new QHBoxLayout(connectionGroup_);
    
    connectButton_ = new QPushButton("Подключиться", this);
    disconnectButton_ = new QPushButton("Отключиться", this);
    connectionStatus_ = new QLabel("Не подключен", this);
    
    connectionLayout_->addWidget(connectButton_);
    connectionLayout_->addWidget(disconnectButton_);
    connectionLayout_->addWidget(connectionStatus_);
    connectionLayout_->addStretch();
    
    // Группа Server1
    server1Group_ = new QGroupBox("Server1 - Информация о системе", this);
    server1Layout_ = new QGridLayout(server1Group_);
    
    diskInfoButton_ = new QPushButton("Получить информацию о дисках", this);
    filesystemInfoButton_ = new QPushButton("Получить информацию о ФС", this);
    keyboardInfoButton_ = new QPushButton("Получить информацию о клавиатуре", this);
    server1Output_ = new QTextEdit(this);
    server1Output_->setReadOnly(true);
    
    server1Layout_->addWidget(diskInfoButton_, 0, 0);
    server1Layout_->addWidget(filesystemInfoButton_, 0, 1);
    server1Layout_->addWidget(keyboardInfoButton_, 0, 2);
    server1Layout_->addWidget(server1Output_, 1, 0, 1, 3);
    
    // Группа Server2
    server2Group_ = new QGroupBox("Server2 - Информация о процессах", this);
    server2Layout_ = new QGridLayout(server2Group_);
    
    processInfoButton_ = new QPushButton("Получить информацию о процессе", this);
    threadInfoButton_ = new QPushButton("Получить информацию о потоках", this);
    server2Output_ = new QTextEdit(this);
    server2Output_->setReadOnly(true);
    
    server2Layout_->addWidget(processInfoButton_, 0, 0);
    server2Layout_->addWidget(threadInfoButton_, 0, 1);
    server2Layout_->addWidget(server2Output_, 1, 0, 1, 2);
    
    // Добавление групп в основной макет
    mainLayout_->addWidget(connectionGroup_);
    mainLayout_->addWidget(server1Group_);
    mainLayout_->addWidget(server2Group_);
    
    // Статусная строка
    statusBar_ = statusBar();
    progressBar_ = new QProgressBar(this);
    progressBar_->setVisible(false);
    statusBar_->addPermanentWidget(progressBar_);
    
    // Настройка окна
    setWindowTitle("CS-App Client");
    setMinimumSize(800, 600);
    resize(1000, 700);
}

void MainWindow::setupConnections() {
    connect(connectButton_, &QPushButton::clicked, this, &MainWindow::connectToServers);
    connect(disconnectButton_, &QPushButton::clicked, this, &MainWindow::disconnectFromServers);
    connect(diskInfoButton_, &QPushButton::clicked, this, &MainWindow::requestDiskInfo);
    connect(filesystemInfoButton_, &QPushButton::clicked, this, &MainWindow::requestFilesystemInfo);
    connect(keyboardInfoButton_, &QPushButton::clicked, this, &MainWindow::requestKeyboardInfo);
    connect(processInfoButton_, &QPushButton::clicked, this, &MainWindow::requestProcessInfo);
    connect(threadInfoButton_, &QPushButton::clicked, this, &MainWindow::requestThreadInfo);
}

void MainWindow::connectToServers() {
    if (!client_) {
        client_ = std::make_unique<Client>();
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0); // Неопределенный прогресс
    
    if (client_->connect()) {
        connectionStatus_->setText("Подключен");
        connectionStatus_->setStyleSheet("color: green;");
        statusBar_->showMessage("Успешно подключен к серверам", 3000);
    } else {
        connectionStatus_->setText("Ошибка подключения");
        connectionStatus_->setStyleSheet("color: red;");
        QMessageBox::warning(this, "Ошибка", "Не удалось подключиться к серверам");
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::disconnectFromServers() {
    if (client_) {
        client_->disconnect();
        connectionStatus_->setText("Не подключен");
        connectionStatus_->setStyleSheet("color: black;");
        statusBar_->showMessage("Отключен от серверов", 3000);
    }
}

void MainWindow::requestDiskInfo() {
    if (!client_ || !client_->isConnected()) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к серверам");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);
    
    try {
        auto diskInfos = client_->getDiskInfo();
        
        QString output = "Информация о дисках:\n";
        output += "Время запроса: " + QDateTime::currentDateTime().toString() + "\n\n";
        
        for (const auto& info : diskInfos) {
            output += "Устройство: " + QString::fromStdString(info.device_name) + "\n";
            output += "Точка монтирования: " + QString::fromStdString(info.mount_point) + "\n";
            output += "Тип ФС: " + QString::fromStdString(info.filesystem_type) + "\n";
            output += "Общий размер: " + QString::number(info.total_size) + " байт\n";
            output += "Свободно: " + QString::number(info.free_size) + " байт\n";
            output += "---\n";
        }
        
        server1Output_->setPlainText(output);
        server1Output_->verticalScrollBar()->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка получения информации о дисках: %1").arg(e.what()));
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::requestFilesystemInfo() {
    if (!client_ || !client_->isConnected()) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к серверам");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);
    
    try {
        auto fsInfo = client_->getFilesystemInfo();
        
        QString output = "Информация о файловой системе:\n";
        output += "Время запроса: " + QDateTime::currentDateTime().toString() + "\n\n";
        output += "Тип: " + QString::fromStdString(fsInfo.type) + "\n";
        output += "Точка монтирования: " + QString::fromStdString(fsInfo.mount_point) + "\n";
        output += "Размер блока: " + QString::number(fsInfo.block_size) + " байт\n";
        output += "Всего блоков: " + QString::number(fsInfo.total_blocks) + "\n";
        output += "Свободно блоков: " + QString::number(fsInfo.free_blocks) + "\n";
        
        server1Output_->setPlainText(output);
        server1Output_->verticalScrollBar()->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка получения информации о ФС: %1").arg(e.what()));
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::requestKeyboardInfo() {
    if (!client_ || !client_->isConnected()) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к серверам");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);
    
    try {
        auto kbInfo = client_->getKeyboardInfo();
        
        QString output = "Информация о клавиатуре:\n";
        output += "Время запроса: " + QDateTime::currentDateTime().toString() + "\n\n";
        output += "Раскладка: " + QString::fromStdString(kbInfo.layout) + "\n";
        output += "Вариант: " + QString::fromStdString(kbInfo.variant) + "\n";
        output += "Модель: " + QString::fromStdString(kbInfo.model) + "\n";
        
        server1Output_->setPlainText(output);
        server1Output_->verticalScrollBar()->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка получения информации о клавиатуре: %1").arg(e.what()));
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::requestProcessInfo() {
    if (!client_ || !client_->isConnected()) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к серверам");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);
    
    try {
        auto procInfo = client_->getProcessInfo();
        
        QString output = "Информация о процессе:\n";
        output += "Время запроса: " + QDateTime::currentDateTime().toString() + "\n\n";
        output += "PID: " + QString::number(procInfo.pid) + "\n";
        output += "Имя: " + QString::fromStdString(procInfo.name) + "\n";
        output += "Пользователь: " + QString::fromStdString(procInfo.user) + "\n";
        output += "Использование памяти: " + QString::number(procInfo.memory_usage) + " байт\n";
        output += "Использование CPU: " + QString::number(procInfo.cpu_usage, 'f', 2) + "%\n";
        
        server2Output_->setPlainText(output);
        server2Output_->verticalScrollBar()->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка получения информации о процессе: %1").arg(e.what()));
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::requestThreadInfo() {
    if (!client_ || !client_->isConnected()) {
        QMessageBox::warning(this, "Ошибка", "Сначала подключитесь к серверам");
        return;
    }
    
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0);
    
    try {
        auto threadInfos = client_->getThreadInfo();
        
        QString output = "Информация о потоках:\n";
        output += "Время запроса: " + QDateTime::currentDateTime().toString() + "\n\n";
        output += "Количество потоков: " + QString::number(threadInfos.size()) + "\n\n";
        
        for (const auto& info : threadInfos) {
            output += "TID: " + QString::number(info.tid) + "\n";
            output += "Имя: " + QString::fromStdString(info.name) + "\n";
            output += "Приоритет: " + QString::number(info.priority) + "\n";
            output += "Состояние: " + QString::fromStdString(info.state) + "\n";
            output += "---\n";
        }
        
        server2Output_->setPlainText(output);
        server2Output_->verticalScrollBar()->setValue(0);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка получения информации о потоках: %1").arg(e.what()));
    }
    
    progressBar_->setVisible(false);
}

void MainWindow::updateStatus() {
    if (client_ && client_->isConnected()) {
        statusBar_->showMessage("Подключен к серверам", 0);
    } else {
        statusBar_->showMessage("Не подключен", 0);
    }
}

