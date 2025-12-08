
#pragma once
#include <QtWidgets>

class BLEController;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void startScan();
    void sendCommand();

public:
    void log(const QString& line);
    void clearBle();

private:
    // UI
    QPushButton* scanBtn_{};
    QTextEdit* logView_{};
    QLineEdit* cmdEdit_{};
    QPushButton* sendBtn_{};
    QLabel* statusLbl_{};


    // BLE
    BLEController *bleCtrl = nullptr;
};
