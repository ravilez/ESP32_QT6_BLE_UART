#include "cspxtableview.h"

#include <QKeyEvent>
#include <QStandardItem>

CSPXTableView::CSPXTableView(QWidget *parent)
    :QTableView(parent)
{

}

void CSPXTableView::keyPressEvent( QKeyEvent *e )
{
    if (e->type() == QEvent::KeyPress)
    {
        if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
        {
            int selectedRow = currentIndex().row();
            if (e->key() == Qt::Key_Down)
            {
                if(selectedRow == model()->rowCount()-1)
                {
                   return;
                }
                QList<QStandardItem *> list = itemModel->takeRow(currentIndex().row());
                itemModel->insertRow(selectedRow+1,list);
                QModelIndex index = model()->index(selectedRow + 1, 0);
                setCurrentIndex(index);
            }
            else
            {
                if(selectedRow == 0)
                {
                   return;
                }
                QList<QStandardItem *> list = itemModel->takeRow(currentIndex().row());
                itemModel->insertRow(selectedRow-1,list);
                QModelIndex index = model()->index(selectedRow - 1, 0);
                setCurrentIndex(index);
            }
        }
    }
}
