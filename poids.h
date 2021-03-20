#ifndef POIDS_H
#define POIDS_H

#include<QGraphicsItem>
#include<arete.h>
#include <QGraphicsObject>

class arete;
class Poids: public QGraphicsTextItem
{
public:

    friend class arete;
    Poids();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // POIDS_H
