#include "sommet.h"
#include "arete.h"
#include "graphe.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include "mainwindow.h"


class arete;
static int c=0;
sommet::sommet(): id(),active(false)
{
    this->id = c++;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    update();
}

sommet::sommet(int val): id(),active(false)
{
    this->id = val;
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    update();
}

void sommet::addEdge(arete *edge)
{
    listeAretes << edge;
    edge->adjust();

}

QVector<arete *> sommet::edges() const
{
    return listeAretes;
}

bool sommet::advancePosition()
{
    if (newPos == pos()) return false;

    setPos(newPos);
    return true;
}

QRectF sommet::boundingRect() const
{
    qreal adjust;
    if(!Mode){
        adjust=20;
    }
    if(Mode){
        adjust=100;
    }
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath sommet::shape() const
{
    QPainterPath path;
    path.addEllipse(-50, -50, 70, 70);
    return path;
}

void sommet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing,true);
    (void)option; (void)widget;
    QRectF rect = boundingRect();

    QPen pen(Qt::black, 2);
    //painter->setBrush(Qt::yellow);


    QPixmap pin("./map/pinRouge.svg");
    QPixmap pin2("./map/pinJaune.svg");
    painter->setPen(pen);
    if(Mode){
        if(!active)
        {

            painter->drawPixmap(-50,-50,70,70,pin);
        }

        //painter->drawPixmap(-50,-50,70,70,pin);
        if(active)
        {
            //painter->setBrush(Qt::red);
            painter->drawPixmap(-50,-50,70,70,pin2);
        }
        QFont font("Arial");
        painter->setFont(font);
        painter->scale(2,2);
        QString a=QString::number(id);

        painter->drawText(-10,-8,a);
    }
    if(!Mode){
        if(!active)
        {
            painter->setBrush(Qt::yellow);
            painter->drawEllipse(-16,-16,24,24);
        }


        if(active)
        {
            painter->setBrush(Qt::red);
            painter->drawEllipse(-16,-16,24,24);
        }
        QFont font("Arial");
        painter->setFont(font);

        QString a=QString::number(id);

        painter->drawText(-4,0,a);
    }



}

QVariant sommet::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (arete *edge : qAsConst(listeAretes)) edge->adjust(); break;
    default: break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void sommet::deactivate()
{
    active = false;
    update();
}


void sommet::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    //update();

    QGraphicsItem::mouseDoubleClickEvent(event);
    active = !active;
    update();

    if(active)
    {
        emit this->activated(this);
    }
    else
    {
        emit this->deactivated(this);
    }
}

int sommet::getId() const
{
    return id;
}

arete *sommet::edgeTo(sommet *nxt)
{
    if (!nxt) return nullptr;
    for (arete *aret : arc_sortant)
        if (*(aret->sommets().second) == *nxt) return aret;
    return nullptr;
}

bool sommet::operator== (const sommet& other) const
{
    return getId() == other.getId();
}


