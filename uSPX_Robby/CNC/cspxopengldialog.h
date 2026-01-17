#ifndef CSPXOPENGLDIALOG_H
#define CSPXOPENGLDIALOG_H

#include <QDialog>
#include <QOpenGLFunctions>
#include <QTreeWidget>

#include "cspxopenglwidget.h"
#include "cspxhpglcommands.h"

namespace Ui {
class CSPXOpenGLDialog;
}

class CSPXOpenGLDialog : public QDialog, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CSPXOpenGLDialog(QWidget *parent = nullptr);
    ~CSPXOpenGLDialog();

    CSPXOpenGLWidget* GetOpenGLWidget();
    void createOpenGLVertices(QVector<QVector3D> &polyLine);

    void setCNCData(CSPXHpglCommands *hpglData);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_flipY_clicked();

    void on_flipX_clicked();

    void on_updateCNC_clicked();

    void on_rotate_clicked();

    void on_move2Spindle_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_xPosSpinBox_valueChanged(int arg1);

    void on_yPosSpinBox_valueChanged(int arg1);

private:
    Ui::CSPXOpenGLDialog *ui;

    CSPXOpenGLWidget *glWidget = nullptr;

    CSPXHpglCommands *hpglCommands = nullptr;
    int x_min;
    int y_min;
    int z_min;
    int x_max;
    int y_max;
    int z_max;
};

#endif // CSPXOPENGLDIALOG_H
