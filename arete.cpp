#include "arete.h"
#include "sommet.h"
#include <QPainter>
#include <QtMath>
#include <QMessageBox>
#include"mainwindow.h"
#include <QPushButton>
#include<QMenu>
#include<QAction>
#include<QGraphicsSceneMouseEvent>
#include <QDebug>

arete::arete(sommet *source, sommet *destination,int poids)
    : src(source), dest(destination),poid(poids)
{
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
}
bool arete::operator==(const arete &a){
    return ((this->getSrc()->getId())==(a.getSrc()->getId()) && (this->getDest()->getId())==(a.getSrc()->getId()) && (this->getPoid())==(a.poid) );
}

bool arete::operator<(const arete &areete)const{
    return ((this->poid)<(areete.poid));
}
bool arete::operator>(const arete &areete)const{
    return ((this->poid)>(areete.poid));
}
bool arete::operator<=(const arete &areete)const{
    return ((this->poid)<=(areete.poid));
}
bool arete::operator>=(const arete &areete)const{
    return ((this->poid)>=(areete.poid));
}

void arete::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::RightButton){
        QMenu *menu2=new QMenu("bla");
        QAction *action=new QAction("Changer le Poids");
        QAction *action2=new QAction("Supprimer");
        menu2->addAction(action);
        menu2->addAction(action2);
        QObject::connect(action,SIGNAL(triggered()),this,SLOT(askpoid()));
        QObject::connect(action2,SIGNAL(triggered()),this,SLOT(deletearete()));
        menu2->exec(cursor().pos());}
}
void arete::askpoid(){
    bool ok = false;
    int newPoid = QInputDialog::getInt(nullptr, "poids", "Quel est le poids ?", poid, 0,2000,1, &ok);
    setPoid(newPoid);

    update();
}
void arete::deletearete(){
    int i;
    this->suprim=true;
    for(i=0;i<this->src->listeAretes.size();i++){
        if(this->src->listeAretes[i]==this) this->src->listeAretes.remove(i);
    }
    for(i=0;i<this->dest->listeAretes.size();i++){
        if(this->dest->listeAretes[i]==this) this->dest->listeAretes.remove(i);
    }
    for(i=0;i<this->src->voisins.size();i++){
        if(this->src->voisins[i]==this->dest) this->src->voisins.remove(i);
    }
    for(i=0;i<this->dest->voisins.size();i++){
        if(this->dest->voisins[i]==this->src) this->dest->voisins.remove(i);
    }
    /*for(i=0;i<mon_graphe->Edges.size();i++){
        qInfo()<<"je suis entré";
        if(this==mon_graphe->Edges[i]){
            mon_graphe->Edges.remove(i);
            break;
        }
    }
    for(i=0;i<mon_graphe->Pgraph.size();i++){
        if(this==mon_graphe->Pgraph[i]){
            mon_graphe->Pgraph.remove(i);
            break;
        }
    }*/

    scene()->removeItem( this);
    update();
}
int arete::getPoid(){
    return poid;
}

void arete::setPoid(int p){
    poid = p;
}

sommet *arete::getSrc() const{
    return src;
}

sommet *arete::getDest() const{
    return dest;
}

QPair<sommet *, sommet *> arete::sommets() const{
    return QPair<sommet *, sommet *>(src , dest);
}

void arete::adjust()
{
    if (!src || !dest) return;

    QLineF line(mapFromItem(src, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    }
    else {
        sourcePoint = destPoint = line.p1();
    }
}

QRectF arete::boundingRect() const
{
    if (!src || !dest) return QRectF();
    qreal penWidth = 1;
    qreal extra = (penWidth)*10;

    return QRectF((sourcePoint+destPoint)/2, QSizeF(2,
                                                    2)).adjusted(-extra, -extra, extra, extra);
}

void arete::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!src || !dest) return;
    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.))) return;
    if( coloring==false){
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));}
    if(coloring==true){
        painter->setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }
    painter->drawLine(line);

    QString s = QString::number(poid);
    QFont font("Arial");
    //font.setPointSize(font.pointSize()*2);
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    painter->setFont(font);

    scene()->update();
    painter->drawText((sourcePoint+destPoint)/2,s);
}



