/********************************************************************************
** Form generated from reading UI file 'mainwindowvblNbz.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWVBLNBZ_H
#define MAINWINDOWVBLNBZ_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "CNC/cspxgraphicsview.h"
#include "CNC/cspxtableview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *positionGroupBox;
    QLabel *z_label;
    QLabel *y_label;
    QLabel *x_label;
    QLCDNumber *x_lcdNumber;
    QLCDNumber *y_lcdNumber;
    QLCDNumber *z_lcdNumber;
    QGroupBox *stepGroupBox;
    QLineEdit *xStep;
    QLabel *dx_label;
    QLineEdit *yStep;
    QLineEdit *zStep;
    QLabel *dy_label;
    QLabel *dz_label;
    QCheckBox *toolSafetyCheckBox;
    QPushButton *homeButton;
    QPushButton *downButton;
    QPushButton *leftButton;
    QPushButton *upButton;
    QPushButton *rightButton;
    QGroupBox *homeGroupBox;
    QLabel *zhome_label;
    QLabel *yhome_label;
    QLabel *xhome_label;
    QLCDNumber *xHome;
    QLCDNumber *yHome;
    QLCDNumber *zHome;
    QLabel *robby_label;
    QPushButton *connectButton;
    QGroupBox *groupBox;
    QRadioButton *fileMode;
    QRadioButton *localMode;
    QRadioButton *dccMode;
    QRadioButton *lineMode;
    QComboBox *serialComboBox;
    QPushButton *zupButton;
    QPushButton *zdownButton;
    QTextEdit *statusMessage;
    QPushButton *leftupButton;
    QPushButton *leftdownButton;
    QPushButton *rightupButton;
    QPushButton *rightdownButton;
    QPushButton *openFileButton;
    QLabel *label_Filename;
    QLabel *label_CTS_Status;
    QPushButton *zhomeButton;
    QPushButton *resetButton;
    QPushButton *SystemButton;
    QLabel *CTS_label;
    QPushButton *sendFileButton;
    QPushButton *modeButton;
    QPushButton *setXYHomeButton;
    QPushButton *setZHomeButton;
    QPushButton *setXYZHomeButton;
    QPushButton *viewSourceFile;
    QLabel *label_Busy_Status;
    QLabel *busy_label;
    QLineEdit *zSafetyStep;
    QPushButton *moveWorkpieceToXYHomePositionButton;
    QLabel *x0;
    QLabel *xOrigin;
    QLabel *yOrigin;
    QLabel *y0;
    QPushButton *HelpButton;
    QComboBox *savedPositionBox;
    QLabel *savedPositions_label;
    QLabel *travelSpeed_label;
    QDoubleSpinBox *travelSpeedSpinBox;
    QLabel *origin_label;
    QLabel *xyHome_label;
    QLabel *dx_label_2;
    QPushButton *closeFileButton;
    CSPXTableView *tableView;
    CSPXGraphicsView *workspace;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(801, 555);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        positionGroupBox = new QGroupBox(centralwidget);
        positionGroupBox->setObjectName("positionGroupBox");
        positionGroupBox->setGeometry(QRect(170, 10, 161, 91));
        z_label = new QLabel(positionGroupBox);
        z_label->setObjectName("z_label");
        z_label->setGeometry(QRect(100, 60, 16, 16));
        y_label = new QLabel(positionGroupBox);
        y_label->setObjectName("y_label");
        y_label->setGeometry(QRect(100, 40, 21, 16));
        x_label = new QLabel(positionGroupBox);
        x_label->setObjectName("x_label");
        x_label->setGeometry(QRect(100, 20, 21, 16));
        x_lcdNumber = new QLCDNumber(positionGroupBox);
        x_lcdNumber->setObjectName("x_lcdNumber");
        x_lcdNumber->setGeometry(QRect(10, 20, 81, 23));
        x_lcdNumber->setAutoFillBackground(false);
        x_lcdNumber->setSmallDecimalPoint(false);
        x_lcdNumber->setDigitCount(7);
        y_lcdNumber = new QLCDNumber(positionGroupBox);
        y_lcdNumber->setObjectName("y_lcdNumber");
        y_lcdNumber->setGeometry(QRect(10, 40, 81, 23));
        y_lcdNumber->setAutoFillBackground(false);
        y_lcdNumber->setSmallDecimalPoint(false);
        y_lcdNumber->setDigitCount(7);
        z_lcdNumber = new QLCDNumber(positionGroupBox);
        z_lcdNumber->setObjectName("z_lcdNumber");
        z_lcdNumber->setGeometry(QRect(10, 60, 81, 23));
        z_lcdNumber->setAutoFillBackground(false);
        z_lcdNumber->setSmallDecimalPoint(false);
        z_lcdNumber->setDigitCount(7);
        stepGroupBox = new QGroupBox(centralwidget);
        stepGroupBox->setObjectName("stepGroupBox");
        stepGroupBox->setGeometry(QRect(10, 120, 101, 91));
        xStep = new QLineEdit(stepGroupBox);
        xStep->setObjectName("xStep");
        xStep->setGeometry(QRect(10, 20, 61, 20));
        xStep->setReadOnly(false);
        dx_label = new QLabel(stepGroupBox);
        dx_label->setObjectName("dx_label");
        dx_label->setGeometry(QRect(80, 20, 21, 16));
        yStep = new QLineEdit(stepGroupBox);
        yStep->setObjectName("yStep");
        yStep->setGeometry(QRect(10, 40, 61, 20));
        yStep->setReadOnly(false);
        zStep = new QLineEdit(stepGroupBox);
        zStep->setObjectName("zStep");
        zStep->setGeometry(QRect(10, 60, 61, 20));
        zStep->setReadOnly(false);
        dy_label = new QLabel(stepGroupBox);
        dy_label->setObjectName("dy_label");
        dy_label->setGeometry(QRect(80, 40, 21, 16));
        dz_label = new QLabel(stepGroupBox);
        dz_label->setObjectName("dz_label");
        dz_label->setGeometry(QRect(80, 60, 21, 16));
        toolSafetyCheckBox = new QCheckBox(centralwidget);
        toolSafetyCheckBox->setObjectName("toolSafetyCheckBox");
        toolSafetyCheckBox->setGeometry(QRect(20, 230, 111, 20));
        toolSafetyCheckBox->setChecked(true);
        homeButton = new QPushButton(centralwidget);
        homeButton->setObjectName("homeButton");
        homeButton->setGeometry(QRect(200, 150, 32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/Images/XYHome.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homeButton->setIcon(icon);
        homeButton->setIconSize(QSize(32, 32));
        downButton = new QPushButton(centralwidget);
        downButton->setObjectName("downButton");
        downButton->setGeometry(QRect(200, 180, 32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/DownArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        downButton->setIcon(icon1);
        downButton->setIconSize(QSize(32, 32));
        leftButton = new QPushButton(centralwidget);
        leftButton->setObjectName("leftButton");
        leftButton->setGeometry(QRect(170, 150, 32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/Images/LeftArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        leftButton->setIcon(icon2);
        leftButton->setIconSize(QSize(32, 32));
        leftButton->setAutoDefault(true);
        leftButton->setFlat(false);
        upButton = new QPushButton(centralwidget);
        upButton->setObjectName("upButton");
        upButton->setGeometry(QRect(200, 120, 32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/Images/UpArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        upButton->setIcon(icon3);
        upButton->setIconSize(QSize(32, 32));
        rightButton = new QPushButton(centralwidget);
        rightButton->setObjectName("rightButton");
        rightButton->setGeometry(QRect(230, 150, 32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resource/Images/RightArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        rightButton->setIcon(icon4);
        rightButton->setIconSize(QSize(32, 32));
        homeGroupBox = new QGroupBox(centralwidget);
        homeGroupBox->setObjectName("homeGroupBox");
        homeGroupBox->setGeometry(QRect(10, 10, 151, 91));
        zhome_label = new QLabel(homeGroupBox);
        zhome_label->setObjectName("zhome_label");
        zhome_label->setGeometry(QRect(100, 60, 16, 16));
        yhome_label = new QLabel(homeGroupBox);
        yhome_label->setObjectName("yhome_label");
        yhome_label->setGeometry(QRect(100, 40, 21, 16));
        xhome_label = new QLabel(homeGroupBox);
        xhome_label->setObjectName("xhome_label");
        xhome_label->setGeometry(QRect(100, 20, 21, 16));
        xHome = new QLCDNumber(homeGroupBox);
        xHome->setObjectName("xHome");
        xHome->setGeometry(QRect(10, 20, 81, 23));
        xHome->setAutoFillBackground(false);
        xHome->setSmallDecimalPoint(false);
        xHome->setDigitCount(7);
        yHome = new QLCDNumber(homeGroupBox);
        yHome->setObjectName("yHome");
        yHome->setGeometry(QRect(10, 40, 81, 23));
        yHome->setAutoFillBackground(false);
        yHome->setSmallDecimalPoint(false);
        yHome->setDigitCount(7);
        zHome = new QLCDNumber(homeGroupBox);
        zHome->setObjectName("zHome");
        zHome->setGeometry(QRect(10, 60, 81, 23));
        zHome->setAutoFillBackground(false);
        zHome->setSmallDecimalPoint(false);
        zHome->setDigitCount(7);
        robby_label = new QLabel(centralwidget);
        robby_label->setObjectName("robby_label");
        robby_label->setEnabled(true);
        robby_label->setGeometry(QRect(560, 10, 91, 71));
        robby_label->setStyleSheet(QString::fromUtf8("background-image: url(:/Images/Profiler.png);"));
        robby_label->setPixmap(QPixmap(QString::fromUtf8(":/resource/Images/Profiler.png")));
        robby_label->setScaledContents(true);
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(698, 60, 81, 31));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(550, 120, 120, 111));
        fileMode = new QRadioButton(groupBox);
        fileMode->setObjectName("fileMode");
        fileMode->setGeometry(QRect(10, 80, 101, 17));
        localMode = new QRadioButton(groupBox);
        localMode->setObjectName("localMode");
        localMode->setGeometry(QRect(10, 20, 101, 17));
        localMode->setChecked(true);
        dccMode = new QRadioButton(groupBox);
        dccMode->setObjectName("dccMode");
        dccMode->setGeometry(QRect(10, 40, 101, 17));
        lineMode = new QRadioButton(groupBox);
        lineMode->setObjectName("lineMode");
        lineMode->setGeometry(QRect(10, 60, 101, 17));
        serialComboBox = new QComboBox(centralwidget);
        serialComboBox->setObjectName("serialComboBox");
        serialComboBox->setGeometry(QRect(698, 30, 81, 22));
        zupButton = new QPushButton(centralwidget);
        zupButton->setObjectName("zupButton");
        zupButton->setGeometry(QRect(120, 120, 32, 32));
        zupButton->setIcon(icon3);
        zupButton->setIconSize(QSize(32, 32));
        zdownButton = new QPushButton(centralwidget);
        zdownButton->setObjectName("zdownButton");
        zdownButton->setGeometry(QRect(120, 180, 32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resource/Images/DownArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        zdownButton->setIcon(icon5);
        zdownButton->setIconSize(QSize(32, 32));
        statusMessage = new QTextEdit(centralwidget);
        statusMessage->setObjectName("statusMessage");
        statusMessage->setGeometry(QRect(580, 400, 201, 131));
        statusMessage->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        statusMessage->setReadOnly(false);
        leftupButton = new QPushButton(centralwidget);
        leftupButton->setObjectName("leftupButton");
        leftupButton->setGeometry(QRect(170, 120, 32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resource/Images/LeftUpArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        leftupButton->setIcon(icon6);
        leftupButton->setIconSize(QSize(32, 32));
        leftupButton->setAutoDefault(true);
        leftupButton->setFlat(false);
        leftdownButton = new QPushButton(centralwidget);
        leftdownButton->setObjectName("leftdownButton");
        leftdownButton->setGeometry(QRect(170, 180, 32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resource/Images/LeftDownArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        leftdownButton->setIcon(icon7);
        leftdownButton->setIconSize(QSize(32, 32));
        leftdownButton->setAutoDefault(true);
        leftdownButton->setFlat(false);
        rightupButton = new QPushButton(centralwidget);
        rightupButton->setObjectName("rightupButton");
        rightupButton->setGeometry(QRect(230, 120, 32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resource/Images/RightUpArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        rightupButton->setIcon(icon8);
        rightupButton->setIconSize(QSize(32, 32));
        rightupButton->setAutoDefault(true);
        rightupButton->setFlat(false);
        rightdownButton = new QPushButton(centralwidget);
        rightdownButton->setObjectName("rightdownButton");
        rightdownButton->setGeometry(QRect(230, 180, 32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resource/Images/RightDownArrow.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        rightdownButton->setIcon(icon9);
        rightdownButton->setIconSize(QSize(32, 32));
        rightdownButton->setAutoDefault(true);
        rightdownButton->setFlat(false);
        openFileButton = new QPushButton(centralwidget);
        openFileButton->setObjectName("openFileButton");
        openFileButton->setGeometry(QRect(698, 130, 81, 31));
        label_Filename = new QLabel(centralwidget);
        label_Filename->setObjectName("label_Filename");
        label_Filename->setGeometry(QRect(20, 340, 281, 16));
        label_Filename->setTextInteractionFlags(Qt::TextInteractionFlag::LinksAccessibleByMouse|Qt::TextInteractionFlag::TextEditable);
        label_CTS_Status = new QLabel(centralwidget);
        label_CTS_Status->setObjectName("label_CTS_Status");
        label_CTS_Status->setGeometry(QRect(200, 320, 41, 21));
        zhomeButton = new QPushButton(centralwidget);
        zhomeButton->setObjectName("zhomeButton");
        zhomeButton->setGeometry(QRect(120, 150, 32, 32));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resource/Images/ZHome.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        zhomeButton->setIcon(icon10);
        zhomeButton->setIconSize(QSize(32, 32));
        resetButton = new QPushButton(centralwidget);
        resetButton->setObjectName("resetButton");
        resetButton->setGeometry(QRect(110, 300, 81, 31));
        SystemButton = new QPushButton(centralwidget);
        SystemButton->setObjectName("SystemButton");
        SystemButton->setGeometry(QRect(20, 300, 81, 31));
        CTS_label = new QLabel(centralwidget);
        CTS_label->setObjectName("CTS_label");
        CTS_label->setGeometry(QRect(200, 300, 31, 16));
        sendFileButton = new QPushButton(centralwidget);
        sendFileButton->setObjectName("sendFileButton");
        sendFileButton->setGeometry(QRect(698, 210, 81, 31));
        modeButton = new QPushButton(centralwidget);
        modeButton->setObjectName("modeButton");
        modeButton->setGeometry(QRect(550, 250, 81, 31));
        setXYHomeButton = new QPushButton(centralwidget);
        setXYHomeButton->setObjectName("setXYHomeButton");
        setXYHomeButton->setGeometry(QRect(390, 20, 32, 32));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resource/Images/Images20x20/newhomexy.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        setXYHomeButton->setIcon(icon11);
        setXYHomeButton->setIconSize(QSize(32, 32));
        setZHomeButton = new QPushButton(centralwidget);
        setZHomeButton->setObjectName("setZHomeButton");
        setZHomeButton->setGeometry(QRect(350, 20, 32, 32));
        setZHomeButton->setAutoFillBackground(false);
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/resource/Images/Images20x20/newhomez.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        setZHomeButton->setIcon(icon12);
        setZHomeButton->setIconSize(QSize(32, 32));
        setXYZHomeButton = new QPushButton(centralwidget);
        setXYZHomeButton->setObjectName("setXYZHomeButton");
        setXYZHomeButton->setGeometry(QRect(430, 20, 32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/resource/Images/Images20x20/newhome.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        setXYZHomeButton->setIcon(icon13);
        setXYZHomeButton->setIconSize(QSize(32, 32));
        viewSourceFile = new QPushButton(centralwidget);
        viewSourceFile->setObjectName("viewSourceFile");
        viewSourceFile->setGeometry(QRect(698, 170, 81, 31));
        label_Busy_Status = new QLabel(centralwidget);
        label_Busy_Status->setObjectName("label_Busy_Status");
        label_Busy_Status->setGeometry(QRect(240, 320, 41, 21));
        busy_label = new QLabel(centralwidget);
        busy_label->setObjectName("busy_label");
        busy_label->setGeometry(QRect(240, 300, 31, 16));
        zSafetyStep = new QLineEdit(centralwidget);
        zSafetyStep->setObjectName("zSafetyStep");
        zSafetyStep->setGeometry(QRect(20, 260, 61, 20));
        zSafetyStep->setReadOnly(false);
        moveWorkpieceToXYHomePositionButton = new QPushButton(centralwidget);
        moveWorkpieceToXYHomePositionButton->setObjectName("moveWorkpieceToXYHomePositionButton");
        moveWorkpieceToXYHomePositionButton->setGeometry(QRect(470, 20, 32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/resource/Images/Images20x20/workpiece2XYHome.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        moveWorkpieceToXYHomePositionButton->setIcon(icon14);
        x0 = new QLabel(centralwidget);
        x0->setObjectName("x0");
        x0->setGeometry(QRect(90, 370, 21, 16));
        xOrigin = new QLabel(centralwidget);
        xOrigin->setObjectName("xOrigin");
        xOrigin->setGeometry(QRect(120, 370, 71, 16));
        yOrigin = new QLabel(centralwidget);
        yOrigin->setObjectName("yOrigin");
        yOrigin->setGeometry(QRect(240, 370, 71, 16));
        y0 = new QLabel(centralwidget);
        y0->setObjectName("y0");
        y0->setGeometry(QRect(210, 370, 16, 16));
        HelpButton = new QPushButton(centralwidget);
        HelpButton->setObjectName("HelpButton");
        HelpButton->setGeometry(QRect(698, 290, 81, 31));
        savedPositionBox = new QComboBox(centralwidget);
        savedPositionBox->setObjectName("savedPositionBox");
        savedPositionBox->setGeometry(QRect(300, 320, 101, 25));
        savedPositions_label = new QLabel(centralwidget);
        savedPositions_label->setObjectName("savedPositions_label");
        savedPositions_label->setGeometry(QRect(300, 290, 141, 31));
        travelSpeed_label = new QLabel(centralwidget);
        travelSpeed_label->setObjectName("travelSpeed_label");
        travelSpeed_label->setGeometry(QRect(420, 290, 91, 31));
        travelSpeedSpinBox = new QDoubleSpinBox(centralwidget);
        travelSpeedSpinBox->setObjectName("travelSpeedSpinBox");
        travelSpeedSpinBox->setGeometry(QRect(420, 320, 51, 25));
        travelSpeedSpinBox->setDecimals(1);
        travelSpeedSpinBox->setMinimum(5.000000000000000);
        travelSpeedSpinBox->setMaximum(50.000000000000000);
        travelSpeedSpinBox->setValue(20.000000000000000);
        origin_label = new QLabel(centralwidget);
        origin_label->setObjectName("origin_label");
        origin_label->setGeometry(QRect(20, 366, 63, 21));
        xyHome_label = new QLabel(centralwidget);
        xyHome_label->setObjectName("xyHome_label");
        xyHome_label->setGeometry(QRect(480, 320, 51, 21));
        dx_label_2 = new QLabel(centralwidget);
        dx_label_2->setObjectName("dx_label_2");
        dx_label_2->setGeometry(QRect(700, 10, 81, 20));
        closeFileButton = new QPushButton(centralwidget);
        closeFileButton->setObjectName("closeFileButton");
        closeFileButton->setGeometry(QRect(700, 250, 81, 29));
        tableView = new CSPXTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(20, 400, 541, 131));
        tableView->setDragEnabled(true);
        tableView->setDragDropMode(QAbstractItemView::DragDropMode::InternalMove);
        tableView->setDefaultDropAction(Qt::DropAction::MoveAction);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        workspace = new CSPXGraphicsView(centralwidget);
        workspace->setObjectName("workspace");
        workspace->setGeometry(QRect(350, 70, 151, 201));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(workspace->sizePolicy().hasHeightForWidth());
        workspace->setSizePolicy(sizePolicy);
        workspace->setMouseTracking(true);
        workspace->setAutoFillBackground(true);
        workspace->setStyleSheet(QString::fromUtf8("backgound-color:rgb(64,64,64)"));
        workspace->setFrameShape(QFrame::Shape::Box);
        workspace->setLineWidth(0);
        workspace->setMidLineWidth(0);
        workspace->setSceneRect(QRectF(0, 0, 150, 200));
        workspace->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);
        workspace->setOptimizationFlags(QGraphicsView::OptimizationFlag::DontAdjustForAntialiasing);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        leftButton->setDefault(false);
        leftupButton->setDefault(false);
        leftdownButton->setDefault(false);
        rightupButton->setDefault(false);
        rightdownButton->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Robby CNC Controller", nullptr));
        positionGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Relative Position /mm", nullptr));
        z_label->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        y_label->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        x_label->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        stepGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Steps", nullptr));
        dx_label->setText(QCoreApplication::translate("MainWindow", "dx", nullptr));
        dy_label->setText(QCoreApplication::translate("MainWindow", "dy", nullptr));
        dz_label->setText(QCoreApplication::translate("MainWindow", "dz", nullptr));
        toolSafetyCheckBox->setText(QCoreApplication::translate("MainWindow", "Tool Safety Step", nullptr));
#if QT_CONFIG(tooltip)
        homeButton->setToolTip(QCoreApplication::translate("MainWindow", "XY Home", nullptr));
#endif // QT_CONFIG(tooltip)
        homeButton->setText(QString());
        downButton->setText(QString());
#if QT_CONFIG(shortcut)
        downButton->setShortcut(QCoreApplication::translate("MainWindow", "Down", nullptr));
#endif // QT_CONFIG(shortcut)
        leftButton->setText(QString());
#if QT_CONFIG(shortcut)
        leftButton->setShortcut(QCoreApplication::translate("MainWindow", "Left", nullptr));
#endif // QT_CONFIG(shortcut)
        upButton->setText(QString());
#if QT_CONFIG(shortcut)
        upButton->setShortcut(QCoreApplication::translate("MainWindow", "Up", nullptr));
#endif // QT_CONFIG(shortcut)
        rightButton->setText(QString());
#if QT_CONFIG(shortcut)
        rightButton->setShortcut(QCoreApplication::translate("MainWindow", "Right", nullptr));
#endif // QT_CONFIG(shortcut)
        homeGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Home Position /mm", nullptr));
        zhome_label->setText(QCoreApplication::translate("MainWindow", "z", nullptr));
        yhome_label->setText(QCoreApplication::translate("MainWindow", "y", nullptr));
        xhome_label->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        robby_label->setText(QString());
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "HPGL Mode", nullptr));
        fileMode->setText(QCoreApplication::translate("MainWindow", "File Mode", nullptr));
        localMode->setText(QCoreApplication::translate("MainWindow", "Local Mode", nullptr));
        dccMode->setText(QCoreApplication::translate("MainWindow", "DCC Mode", nullptr));
        lineMode->setText(QCoreApplication::translate("MainWindow", "Line  Mode", nullptr));
#if QT_CONFIG(tooltip)
        zupButton->setToolTip(QCoreApplication::translate("MainWindow", "Z Up", nullptr));
#endif // QT_CONFIG(tooltip)
        zupButton->setText(QString());
#if QT_CONFIG(shortcut)
        zupButton->setShortcut(QCoreApplication::translate("MainWindow", "PgUp", nullptr));
#endif // QT_CONFIG(shortcut)
#if QT_CONFIG(tooltip)
        zdownButton->setToolTip(QCoreApplication::translate("MainWindow", "Z Down", nullptr));
#endif // QT_CONFIG(tooltip)
        zdownButton->setText(QString());
#if QT_CONFIG(shortcut)
        zdownButton->setShortcut(QCoreApplication::translate("MainWindow", "PgDown", nullptr));
#endif // QT_CONFIG(shortcut)
        statusMessage->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt;\">Error messages</span></p></body></html>", nullptr));
        leftupButton->setText(QString());
        leftdownButton->setText(QString());
        rightupButton->setText(QString());
        rightdownButton->setText(QString());
        openFileButton->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        label_Filename->setText(QCoreApplication::translate("MainWindow", "Filename", nullptr));
        label_CTS_Status->setText(QCoreApplication::translate("MainWindow", "CTS", nullptr));
#if QT_CONFIG(tooltip)
        zhomeButton->setToolTip(QCoreApplication::translate("MainWindow", "Z Home", nullptr));
#endif // QT_CONFIG(tooltip)
        zhomeButton->setText(QString());
#if QT_CONFIG(shortcut)
        zhomeButton->setShortcut(QCoreApplication::translate("MainWindow", "PgDown", nullptr));
#endif // QT_CONFIG(shortcut)
        resetButton->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        SystemButton->setText(QCoreApplication::translate("MainWindow", "Config", nullptr));
        CTS_label->setText(QCoreApplication::translate("MainWindow", "CTS", nullptr));
        sendFileButton->setText(QCoreApplication::translate("MainWindow", "Send File", nullptr));
        modeButton->setText(QCoreApplication::translate("MainWindow", " Mode ?", nullptr));
#if QT_CONFIG(tooltip)
        setXYHomeButton->setToolTip(QCoreApplication::translate("MainWindow", "Set XY-Home position", nullptr));
#endif // QT_CONFIG(tooltip)
        setXYHomeButton->setText(QString());
#if QT_CONFIG(tooltip)
        setZHomeButton->setToolTip(QCoreApplication::translate("MainWindow", "Set Z-Home position", nullptr));
#endif // QT_CONFIG(tooltip)
        setZHomeButton->setText(QString());
#if QT_CONFIG(tooltip)
        setXYZHomeButton->setToolTip(QCoreApplication::translate("MainWindow", "Set XYZ-Home position", nullptr));
#endif // QT_CONFIG(tooltip)
        setXYZHomeButton->setText(QString());
        viewSourceFile->setText(QCoreApplication::translate("MainWindow", "Hpgl View", nullptr));
        label_Busy_Status->setText(QCoreApplication::translate("MainWindow", "Busy", nullptr));
        busy_label->setText(QCoreApplication::translate("MainWindow", "Busy", nullptr));
#if QT_CONFIG(tooltip)
        zSafetyStep->setToolTip(QCoreApplication::translate("MainWindow", "z Safety Step", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        moveWorkpieceToXYHomePositionButton->setToolTip(QCoreApplication::translate("MainWindow", "Move workpiece to XY home", nullptr));
#endif // QT_CONFIG(tooltip)
        moveWorkpieceToXYHomePositionButton->setText(QString());
        x0->setText(QCoreApplication::translate("MainWindow", "Xo", nullptr));
        xOrigin->setText(QString());
        yOrigin->setText(QString());
        y0->setText(QCoreApplication::translate("MainWindow", "Yo", nullptr));
        HelpButton->setText(QCoreApplication::translate("MainWindow", "Help", nullptr));
        savedPositions_label->setText(QCoreApplication::translate("MainWindow", "Saved positions", nullptr));
        travelSpeed_label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Travel speed</p></body></html>", nullptr));
        origin_label->setText(QCoreApplication::translate("MainWindow", "Origin", nullptr));
        xyHome_label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p>mms<span style=\" vertical-align:super;\">-1</span></p></body></html>", nullptr));
        dx_label_2->setText(QCoreApplication::translate("MainWindow", "COM Ports", nullptr));
        closeFileButton->setText(QCoreApplication::translate("MainWindow", "Close File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWVBLNBZ_H
