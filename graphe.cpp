#include "graphe.h"
#include "sommet.h"
#include "arete.h"
#include <math.h>
#include<algorithm>
#include <QKeyEvent>
#include <QRandomGenerator>
#include"mainwindow.h"


GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent)
{}

GraphWidget::GraphWidget(QGraphicsScene *scene) :
    ma_scene(scene)
{}

QVector<sommet *> GraphWidget::Nodes()
{
    QVector<sommet *> nodes;
    for(auto &iter: ma_listeAdjacence)
    {
        nodes.push_back(iter.first);
    }
    return nodes;
}

void GraphWidget::addEdge(arete *edge)
{
    sommet *key = edge->sommets().first;
    ma_listeAdjacence[key].push_back(edge);
}

void GraphWidget::addNode(sommet *n)
{
    ma_listeAdjacence[n] = QVector<arete *>();
}

int GraphWidget::numberOfNodes() const
{
    return (int)ma_listeAdjacence.size();
}
bool GraphWidget::connexe(){
    int i;

    for(i=0;i<Nodes().size();i++){
        Nodes()[i]->vis=0;
    }
    QVector<sommet *> stack;
    //QVector<sommet *> visited;
    sommet *current;
    stack.push_back(Nodes()[0]);
    while(stack.size()!=0){
        current=stack[0];
        stack.remove(0);
        for(i=0;i<current->voisins.size();i++){
            if(current->voisins[i]->vis!=1) stack.push_back(current->voisins[i]);
        }
        //visited.push_back(current);
        current->vis=1;

    }
    for(i=0;i<Nodes().size();i++){
        if(Nodes()[i]->vis==0) return false;
    }
    return true;


}
