#ifndef CSPXGRAPHICSVIEW_H
#define CSPXGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class CSPXGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CSPXGraphicsView(QWidget *parent = nullptr) : QGraphicsView(parent) {}
    CSPXGraphicsView();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            QPointF scenePos = mapToScene(event->pos());
            emit clicked(scenePos);
        }
        QGraphicsView::mousePressEvent(event);
    }

signals:
    void clicked(const QPointF &pos);
};


#endif // CSPXGRAPHICSVIEW_H
