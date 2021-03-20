#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <sommet.h>
#include <arete.h>
#include <graphe.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // QVector<arete *> Edges;
    //QVector<arete*> Pgraph;
    QVector<arete *> SortedEdges(QVector<arete *> SortEdges );
    sommet* getNodeById(int id, GraphWidget *graph );
    void randomgraph(int ,int,int maxweight);
    /*#if QT_CONFIG(wheelevent)
        void wheelEvent(QWheelEvent *event) override;
    #endif*/
    void mousePressEvent(QMouseEvent *event) override;
    QVector<arete *> Areteslie(sommet* sommet);
    bool  cyclique(QVector<sommet*> node, QVector<arete*>);
    QVector<arete *> Areteslie(sommet* sommet,QVector<arete *> copieofGraph);
    QVector<arete*> ArbreCouvrant(  int k );
    void k_ArbreCouvrant(int k);
     QVector<arete*> k_arbre(  int k );
    void decolorier();
    bool saveGraphToFile(const QString &saveFileName);
    void createNode(const QPoint& pos,int idtf);
    void createEdge(int firstNodeIndex, int secondNodeIndex, int weight, int sizeNodes);
    bool loadGraphFromFile(const QString &loadFileName);
    void updateAretes();
    QVector<sommet *> ids(QVector<arete*>);
    QString file;
public slots:
    void zoomIn();
    void zoomOut();
    void scale(qreal scaleFactor);
private slots:
    void addNode(QPointF pos);
    void newGraph();
    void nodeActivated(sommet *node);
    void nodeDeactivated(sommet *node);
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void deleteSommet(int id);
    void on_pushButton_2_clicked();
    void on_spinBox_2_valueChanged(int arg1);
    void on_spinBox_3_valueChanged(int arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_3_clicked();
    void open();
    bool save();
    bool saveAs();
    bool saveConfirmation();
    void on_actionOuvrir_triggered();
    void on_actionEnregistrer_triggered();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_10_clicked();
    void on_actionMarrakech_triggered();
    void on_actionMedina_triggered();
    void on_actionAfrique_triggered();
    void on_actionaucune_triggered();
    void on_actionMa_propre_map_image_triggered();
    void on_actionSwitch_Mode_triggered();
    void on_spinBox_4_valueChanged(int arg1);
private:
    Ui::MainWindow *mon_ui;
    QGraphicsScene *ma_scene;
    GraphWidget *mon_graphe;

};

#endif // MAINWINDOW_H
