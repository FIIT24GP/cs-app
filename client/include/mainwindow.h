#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QTimer>
#include <memory>
#include "client.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToServers();
    void disconnectFromServers();
    void requestDiskInfo();
    void requestFilesystemInfo();
    void requestKeyboardInfo();
    void requestProcessInfo();
    void requestThreadInfo();
    void updateStatus();

private:
    void setupUI();
    void setupConnections();
    void updateConnectionStatus();
    
    // UI элементы
    QWidget *centralWidget_;
    QVBoxLayout *mainLayout_;
    
    // Группа подключения
    QGroupBox *connectionGroup_;
    QHBoxLayout *connectionLayout_;
    QPushButton *connectButton_;
    QPushButton *disconnectButton_;
    QLabel *connectionStatus_;
    
    // Группа запросов к Server1
    QGroupBox *server1Group_;
    QGridLayout *server1Layout_;
    QPushButton *diskInfoButton_;
    QPushButton *filesystemInfoButton_;
    QPushButton *keyboardInfoButton_;
    QTextEdit *server1Output_;
    
    // Группа запросов к Server2
    QGroupBox *server2Group_;
    QGridLayout *server2Layout_;
    QPushButton *processInfoButton_;
    QPushButton *threadInfoButton_;
    QTextEdit *server2Output_;
    
    // Статусная строка
    QStatusBar *statusBar_;
    QProgressBar *progressBar_;
    
    // Таймер для обновления статуса
    QTimer *statusTimer_;
    
    // Клиент для работы с серверами
    std::unique_ptr<Client> client_;
};

#include "mainwindow.moc"
