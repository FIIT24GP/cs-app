#include <QApplication>
#include <QMainWindow>
#include "mainwindow.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    try {
        MainWindow window;
        window.show();
        
        return app.exec();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

