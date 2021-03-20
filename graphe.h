#ifndef GRAPHE_H
#define GRAPHE_H

#include <QGraphicsView>
#include<arete.h>


class sommet;
class GraphWidget : public QGraphicsView
{
    Q_OBJECT
public:
    GraphWidget(QWidget *parent = nullptr);
    GraphWidget(QGraphicsScene *scene);
    void itemMoved();
    void addEdge(arete *edge);
    void addNode(sommet *n);
    int numberOfNodes() const;
    QVector<sommet *> Nodes() ;
    bool connexe();
    QVector<arete *> Edges;
    QVector<arete*> Pgraph;
public slots:
private:
    friend class MainWindow;
    class nodeComparator
    {
    public:
        bool operator() (sommet *left, sommet *right) const
        {
            return left->id < right->id;
        }
    };
    std::map<sommet *, QVector<arete *>, nodeComparator> ma_listeAdjacence;
    QGraphicsScene *ma_scene;
};

#endif // GRAPHE_H
