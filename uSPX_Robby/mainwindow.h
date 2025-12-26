#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QGraphicsLineItem>

#include "CNC/cspxrobbycontroller.h"
#include "CNC/cspxhpglcommands.h"
//#include "CNC/cspxhpgldialog.h"
#include "Containers/cspxvector.h"

class FileModeCheckThread : public QThread
{
    Q_OBJECT
protected:
    void run() override;

public:
    void pause();
    void resume();

signals:
    void waitNextTool(const QString &s);
    void finishedProcessing();
    void abortProcessing();
    void sendModeCommand();

public:
    QMutex sync;
    QWaitCondition pauseCond;
    bool _pause;

    MainWindow *dlg;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CSPXRobbyController * GetController() { return &robbyController; }

public:
    QGraphicsScene *scene;
    void drawGrid(const QSize &size, int spacing);
    CSPXVector GetAbsoluteSpindlePosition();

private slots:
    void on_connectButton_clicked();

    void on_homeButton_clicked();

    void on_upButton_clicked();

    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_downButton_clicked();

    void on_zupButton_clicked();

    void on_zdownButton_clicked();

    void on_statusMessage_textChanged();

    void on_lineMode_clicked();

    void on_leftupButton_clicked();

    void on_rightupButton_clicked();

    void on_rightdownButton_clicked();

    void on_leftdownButton_clicked();

    void on_localMode_clicked();

    void on_fileMode_clicked();

    void on_dccMode_clicked();

    void on_openFileButton_clicked();

    void on_zhomeButton_clicked();

    void on_resetButton_clicked();

    void on_SystemButton_clicked();

    void on_sendFileButton_clicked();

    void on_setXYHomeButton_clicked();

    void on_setXYZHomeButton_clicked();

    void on_setZHomeButton_clicked();

    void on_modeButton_clicked();

    void handleToolChange(QString toolInfo);

    void resetWidgets();

    void continue_NextFile();

    void issueModeCommand();

    //void on_viewSourceFile_clicked();

    void on_HelpButton_clicked();

    void on_savedPositionBox_currentTextChanged(const QString &arg1);

    void on_closeFileButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void onImageClicked(const QPointF &pos);

public slots:
    void on_moveWorkpieceToXYHomePositionButton_clicked();
    void updateData(QByteArray* data);
    void hpglDialogClosing();

public:
    void Initialize();
    CSPXRobbyController robbyController;
    //CSPXHPGLDialog *hpglDialog;

    QPixmap *green;
    QPixmap *red;

    QString fileName;
    QByteArray *cncFile;
    CSPXHpglCommands *hpglCommands;
    QStandardItemModel *model;

    FileModeCheckThread *workerThread = nullptr;

    bool abort;
    bool toolReady;
    bool wait_NewHomePosition;

    double last_zHome;
    double last_xHome;
    double last_yHome;

    unsigned int currentTool;

    double xSpindlePos = {0};
    double ySpindlePos = {0};
    QGraphicsLineItem *xSpindleLine = { nullptr };
    QGraphicsLineItem *ySpindleLine = { nullptr };

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
