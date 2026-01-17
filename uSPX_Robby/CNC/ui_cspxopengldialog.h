/********************************************************************************
** Form generated from reading UI file 'cspxopengldialogmXfQrU.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CSPXOPENGLDIALOGMXFQRU_H
#define CSPXOPENGLDIALOGMXFQRU_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTreeWidget>
#include "cspxopenglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_CSPXOpenGLDialog
{
public:
    CSPXOpenGLWidget *openGLWidget;
    QTreeWidget *treeWidget;
    QPushButton *flipX;
    QPushButton *updateCNC;
    QPushButton *flipY;
    QSpinBox *yPosSpinBox;
    QPushButton *move2Spindle;
    QPushButton *rotate;
    QSpinBox *xPosSpinBox;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *CSPXOpenGLDialog)
    {
        if (CSPXOpenGLDialog->objectName().isEmpty())
            CSPXOpenGLDialog->setObjectName("CSPXOpenGLDialog");
        CSPXOpenGLDialog->resize(782, 621);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CSPXOpenGLDialog->sizePolicy().hasHeightForWidth());
        CSPXOpenGLDialog->setSizePolicy(sizePolicy);
        CSPXOpenGLDialog->setSizeGripEnabled(true);
        openGLWidget = new CSPXOpenGLWidget(CSPXOpenGLDialog);
        openGLWidget->setObjectName("openGLWidget");
        openGLWidget->setGeometry(QRect(320, 10, 450, 600));
        sizePolicy.setHeightForWidth(openGLWidget->sizePolicy().hasHeightForWidth());
        openGLWidget->setSizePolicy(sizePolicy);
        openGLWidget->setMouseTracking(true);
        openGLWidget->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
        treeWidget = new QTreeWidget(CSPXOpenGLDialog);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName("treeWidget");
        treeWidget->setGeometry(QRect(10, 100, 301, 511));
        flipX = new QPushButton(CSPXOpenGLDialog);
        flipX->setObjectName("flipX");
        flipX->setGeometry(QRect(80, 10, 75, 24));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(flipX->sizePolicy().hasHeightForWidth());
        flipX->setSizePolicy(sizePolicy1);
        flipX->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("object-flip-horizontal");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8(":/resource/Images/HFlip.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        }
        flipX->setIcon(icon);
        updateCNC = new QPushButton(CSPXOpenGLDialog);
        updateCNC->setObjectName("updateCNC");
        updateCNC->setGeometry(QRect(80, 40, 75, 24));
        sizePolicy1.setHeightForWidth(updateCNC->sizePolicy().hasHeightForWidth());
        updateCNC->setSizePolicy(sizePolicy1);
        flipY = new QPushButton(CSPXOpenGLDialog);
        flipY->setObjectName("flipY");
        flipY->setGeometry(QRect(0, 10, 75, 24));
        sizePolicy1.setHeightForWidth(flipY->sizePolicy().hasHeightForWidth());
        flipY->setSizePolicy(sizePolicy1);
        QIcon icon1;
        iconThemeName = QString::fromUtf8("object-flip-vertical");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8(":/resource/Images/VFlip.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        }
        flipY->setIcon(icon1);
        yPosSpinBox = new QSpinBox(CSPXOpenGLDialog);
        yPosSpinBox->setObjectName("yPosSpinBox");
        yPosSpinBox->setGeometry(QRect(200, 40, 81, 22));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(yPosSpinBox->sizePolicy().hasHeightForWidth());
        yPosSpinBox->setSizePolicy(sizePolicy2);
        yPosSpinBox->setMinimum(-5000);
        yPosSpinBox->setMaximum(5000);
        yPosSpinBox->setSingleStep(10);
        yPosSpinBox->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
        yPosSpinBox->setDisplayIntegerBase(10);
        move2Spindle = new QPushButton(CSPXOpenGLDialog);
        move2Spindle->setObjectName("move2Spindle");
        move2Spindle->setGeometry(QRect(0, 70, 75, 24));
        sizePolicy1.setHeightForWidth(move2Spindle->sizePolicy().hasHeightForWidth());
        move2Spindle->setSizePolicy(sizePolicy1);
        rotate = new QPushButton(CSPXOpenGLDialog);
        rotate->setObjectName("rotate");
        rotate->setGeometry(QRect(0, 40, 75, 24));
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(rotate->sizePolicy().hasHeightForWidth());
        rotate->setSizePolicy(sizePolicy3);
        rotate->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        QIcon icon2;
        iconThemeName = QString::fromUtf8("object-rotate-right");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8(":/resource/Images/rotate_right.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::On);
        }
        rotate->setIcon(icon2);
        rotate->setAutoDefault(true);
        xPosSpinBox = new QSpinBox(CSPXOpenGLDialog);
        xPosSpinBox->setObjectName("xPosSpinBox");
        xPosSpinBox->setGeometry(QRect(200, 10, 81, 22));
        sizePolicy2.setHeightForWidth(xPosSpinBox->sizePolicy().hasHeightForWidth());
        xPosSpinBox->setSizePolicy(sizePolicy2);
        xPosSpinBox->setMinimum(-5000);
        xPosSpinBox->setMaximum(5000);
        xPosSpinBox->setSingleStep(10);
        xPosSpinBox->setStepType(QAbstractSpinBox::StepType::AdaptiveDecimalStepType);
        xPosSpinBox->setValue(0);
        label = new QLabel(CSPXOpenGLDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(180, 14, 16, 16));
        label_2 = new QLabel(CSPXOpenGLDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(180, 43, 16, 16));

        retranslateUi(CSPXOpenGLDialog);

        QMetaObject::connectSlotsByName(CSPXOpenGLDialog);
    } // setupUi

    void retranslateUi(QDialog *CSPXOpenGLDialog)
    {
        CSPXOpenGLDialog->setWindowTitle(QCoreApplication::translate("CSPXOpenGLDialog", "Dialog", nullptr));
        flipX->setText(QCoreApplication::translate("CSPXOpenGLDialog", "Flip X", nullptr));
        updateCNC->setText(QCoreApplication::translate("CSPXOpenGLDialog", "Update CNC", nullptr));
        flipY->setText(QCoreApplication::translate("CSPXOpenGLDialog", "Flip Y", nullptr));
        move2Spindle->setText(QCoreApplication::translate("CSPXOpenGLDialog", "To Spindle", nullptr));
        rotate->setText(QCoreApplication::translate("CSPXOpenGLDialog", "Rotate", nullptr));
        label->setText(QCoreApplication::translate("CSPXOpenGLDialog", "X", nullptr));
        label_2->setText(QCoreApplication::translate("CSPXOpenGLDialog", "Y", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CSPXOpenGLDialog: public Ui_CSPXOpenGLDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CSPXOPENGLDIALOGMXFQRU_H
