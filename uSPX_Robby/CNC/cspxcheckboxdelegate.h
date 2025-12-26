#ifndef CSPXCHECKBOXDELEGATE_H
#define CSPXCHECKBOXDELEGATE_H

#include <QItemDelegate>

class CSPXCheckBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CSPXCheckBoxDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                   const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint ( QPainter * painter , const QStyleOptionViewItem & option , const QModelIndex & index ) const ;        };

#endif // CSPXCHECKBOXDELEGATE_H
