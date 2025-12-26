#include "cspxcheckboxdelegate.h"

#include <QApplication>
#include <QCheckBox>

CSPXCheckBoxDelegate::CSPXCheckBoxDelegate(QObject *parent)
    : QItemDelegate{parent}
{

}

QWidget *CSPXCheckBoxDelegate::createEditor(QWidget *parent,
const QStyleOptionViewItem &/* option */,
const QModelIndex &/* index */) const
{
  QCheckBox *editor = new QCheckBox(parent);
  editor->setCheckState(Qt::Checked);

  return editor;
}

void CSPXCheckBoxDelegate::setEditorData(QWidget *editor,
                                const QModelIndex &index) const
{
    QCheckBox *cBox = static_cast<QCheckBox*>(editor);
    cBox->setChecked(index.data().toBool());
}

void CSPXCheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                               const QModelIndex &index) const
{
    QCheckBox *cBox = static_cast<QCheckBox*>(editor);
    bool value = cBox->checkState();

    model->setData(index, value, Qt::EditRole);
}

void CSPXCheckBoxDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    Q_UNUSED(index );
    QStyleOptionButton checkboxstyle ;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, & checkboxstyle );
    //Centering
    checkboxstyle.rect = option.rect ;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/ 2 - checkbox_rect.width()/ 2 );
    editor ->setGeometry(checkboxstyle.rect );
}

void CSPXCheckBoxDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    //Get data
    bool data = index.model()->data(index, Qt::DisplayRole ). toBool();
    //Create a CheckBox style
    QStyleOptionButton checkboxstyle ;
    QRect checkbox_rect = QApplication::style()->subElementRect(QStyle::SE_CheckBoxIndicator, & checkboxstyle );
    //Centering
    checkboxstyle.rect = option.rect ;
    checkboxstyle.rect.setLeft(option.rect.x() +
                               option.rect.width()/ 2 - checkbox_rect.width()/ 2 );
    //Selected or not selected
    if(data)
        checkboxstyle.state = QStyle::State_On | QStyle::State_Enabled ;
    else
        checkboxstyle.state = QStyle::State_Off | QStyle::State_Enabled ;
    //Done! Display!
    QApplication::style()->drawControl(QStyle::CE_CheckBox, & checkboxstyle, painter );
}
