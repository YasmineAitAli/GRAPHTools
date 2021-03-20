#ifndef SOMMET_H
#define SOMMET_H
#include <QGraphicsItem>
#include <QVector>
#include <QGraphicsObject>


class arete;
class GraphWidget;

class sommet : public QGraphicsObject
{
    Q_OBJECT
signals:
    void activated(sommet* sommet);
    void deactivated(sommet* sommet);
public:
    int vis;
   //int  theSize=0;
    QPointF cursor;
    int id;
    sommet();
    sommet(int val);
    void addEdge(arete *edge);
    arete *edgeTo(sommet *oth);
    QVector<arete *> edges() const;
    bool active;
    bool operator== (const sommet& other) const;
    int getId() const;
    bool Mode=false;
    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    void deactivate();
    bool advancePosition();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVector<sommet *> voisins;
    QVector<arete *> listeAretes;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    friend class arete;
    friend class GraphWidget;
    QPointF newPos;
    GraphWidget *graph;
    QVector<arete *> arc_sortant;
};

#endif // SOMMET_H

