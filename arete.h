#ifndef ARETE_H
#define ARETE_H

#include<QGraphicsItem>
#include<sommet.h>
#include <QGraphicsObject>
#include <QMessageBox>
#include <QWidget>
#include <QInputDialog>
#include <mainwindow.h>
#include<graphe.h>

class sommet;
class arete : public QGraphicsObject
{
    Q_OBJECT
signals:
    void valueChanged();
    void updateValue();
public slots:
    void askpoid();
    void deletearete();
public:
    QPointF sourcePoint;
    QPointF destPoint;
    sommet *src, *dest;
    int poid=10;
    QPair<sommet *, sommet *> sommets() const;
    arete(sommet *source, sommet *destination,int poid);
    sommet *getSrc() const;
    sommet *getDest() const;
    void adjust();
    int getPoid();
    void setPoid(int p);
    bool coloring=false;
    bool operator<(const arete&)const;
    bool operator>(const arete&)const;
    bool operator<=(const arete&)const;
    bool operator>=(const arete&)const;
    bool operator == (arete const& a);
    bool suprim=false;
    GraphWidget *mon_graphe;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override ;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    friend class GraphWidget;
    friend class sommet;
    qreal arrowSize = 10;


};

#endif // ARETE_H
