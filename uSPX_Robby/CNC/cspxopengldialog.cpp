#include "cspxopengldialog.h"
#include "ui_cspxopengldialog.h"

#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVectorIterator>
#include <QBuffer>

CSPXOpenGLDialog::CSPXOpenGLDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSPXOpenGLDialog)
{
    ui->setupUi(this);

    glWidget = ui->openGLWidget;

    setMouseTracking(true);


}

CSPXOpenGLDialog::~CSPXOpenGLDialog()
{
}

void CSPXOpenGLDialog::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    QSize newSize = event->size();
    qDebug() << "Dialog resized to:" << newSize;
    // Add your custom resize handling code here
    ui->openGLWidget->resize(newSize);
}

void CSPXOpenGLDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.restore();
}


void CSPXOpenGLDialog::createOpenGLVertices(QVector<QVector3D> &polyLine)
{
    QVector<QVector3D> *vec3D = glWidget->getVertices();
    vec3D->clear();

    QVectorIterator<QVector3D> it(polyLine);
    while (it.hasNext()) {
         vec3D->append(it.next());
    }

}


CSPXOpenGLWidget* CSPXOpenGLDialog::GetOpenGLWidget()
{
    return ui->openGLWidget;
}

void CSPXOpenGLDialog::setCNCData(CSPXHpglCommands *hpglCmds)
{
    hpglCommands = hpglCmds;

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);


    for (int i = 0; i < hpglCmds->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCmds->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    createOpenGLVertices(hpglCommands->polyLine);

    update();
}


void CSPXOpenGLDialog::on_flipY_clicked()
{
    int x_coord,y_coord;
    int cx = (hpglCommands->x_max+hpglCommands->x_min)/2.;

    QVector<QVector3D> *vec3D = glWidget->getVertices();
    if (vec3D==nullptr)
        return;

    std::for_each(vec3D->begin(), vec3D->end(), [&](QVector3D &v) {v[0] = 2*cx/10000. - v[0];});

    hpglCommands->FlipY();

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);

    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCommands->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    //ui->xPosSpinBox->setValue(x_min);

    ui->openGLWidget->update();

}


void CSPXOpenGLDialog::on_flipX_clicked()
{
    int x_coord,y_coord;
    int cy = (hpglCommands->y_max+hpglCommands->y_min)/2.;

    QVector<QVector3D> *vec3D = glWidget->getVertices();
    if (vec3D==nullptr)
        return;

    std::for_each(vec3D->begin(), vec3D->end(), [&](QVector3D &v) {v[1] = 2*cy/10000. - v[1];});

    hpglCommands->FlipX();

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);

    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCommands->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    //ui->yPosSpinBox->setValue(y_min);

    ui->openGLWidget->update();

}


void CSPXOpenGLDialog::on_updateCNC_clicked()
{

}


void CSPXOpenGLDialog::on_rotate_clicked()
{
    double temp;

    QVector<QVector3D> *vec3D = glWidget->getVertices();
    if (vec3D==nullptr)
        return;

    std::for_each(vec3D->begin(), vec3D->end(), [&](QVector3D &v) {
        temp = v[0];
        v[0] = v[1];
        v[1] = temp;
    });


    hpglCommands->Rotate();

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);

    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCommands->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    //ui->xPosSpinBox->setValue(x_min);
    //ui->yPosSpinBox->setValue(y_min);

    ui->openGLWidget->update();
}


void CSPXOpenGLDialog::on_move2Spindle_clicked()
{

    ui->openGLWidget->update();
}


void CSPXOpenGLDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{

}


void CSPXOpenGLDialog::on_xPosSpinBox_valueChanged(int dx)
{
    QVector<QVector3D> *vec3D = glWidget->getVertices();
    if (vec3D==nullptr)
        return;

    std::for_each(vec3D->begin(), vec3D->end(), [&](QVector3D &v) {v[0] += (dx-hpglCommands->last_dx)/10000.;});

    hpglCommands->TranslateXBy(dx);

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);

    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCommands->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    ui->openGLWidget->update();
}


void CSPXOpenGLDialog::on_yPosSpinBox_valueChanged(int dy)
{
    QVector<QVector3D> *vec3D = glWidget->getVertices();
    if (vec3D==nullptr)
        return;

    std::for_each(vec3D->begin(), vec3D->end(), [&](QVector3D &v) {v[1] += (dy-hpglCommands->last_dy)/10000.;});

    hpglCommands->TranslateYBy(dy);

    ui->treeWidget->clear();

    ui->treeWidget->setHeaderLabel("CBR");
    QTreeWidgetItem *twi = new QTreeWidgetItem();
    twi->setText(0,tr("Files"));
    ui->treeWidget->addTopLevelItem(twi);

    twi = new QTreeWidgetItem();
    twi->setText(0,"Hpgl data");
    ui->treeWidget->topLevelItem(0)->addChild(twi);

    for (int i = 0; i < hpglCommands->fileChunks; i++)
    {
        CSPXListNode<struct CSPXHpglCommands::s_command> *cmdNodes = hpglCommands->commandList[i].GetHead();
        while(cmdNodes)
        {
            struct CSPXHpglCommands::s_command *cmd = cmdNodes->GetElement();
            QTreeWidgetItem *ctwi = new QTreeWidgetItem();
            ctwi->setText(0,cmd->line);
            twi->addChild(ctwi);

            cmdNodes = cmdNodes->GetNext();
        }
    }

    ui->openGLWidget->update();
}

