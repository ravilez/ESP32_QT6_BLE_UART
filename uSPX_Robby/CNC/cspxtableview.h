#ifndef CSPXTABLEVIEW_H
#define CSPXTABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

class CSPXTableView : public QTableView
{
    Q_OBJECT
public:
    CSPXTableView(QWidget *parent = nullptr);


    QStandardItemModel *itemModel = nullptr;

    void keyPressEvent(QKeyEvent *event) override;
};

#endif // CSPXTABLEVIEW_H
