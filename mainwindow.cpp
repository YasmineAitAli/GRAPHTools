#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "arete.h"
#include "sommet.h"
#include <QGraphicsScene>
#include "graphe.h"
#include "QGraphicsView"
#include <QtCore>
#include <QDebug>
#include <QMouseEvent>
#include <QFileDialog>
#include <QtSvg>




sommet *from = nullptr;
sommet *to = nullptr;
int Active = 0;
int arg1 =0;
int arg2 =0;
int arg3 =0;
bool startAdding=false;
bool currentMode=false;
int karbre=0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), mon_ui(new Ui::MainWindow)
{
    mon_ui->setupUi(this);
    mon_graphe = new GraphWidget(ma_scene);
    ma_scene = new QGraphicsScene();




    ma_scene->setItemIndexMethod(QGraphicsScene::NoIndex);


    mon_ui->graphicsView->setScene(ma_scene);
    mon_ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    mon_ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing);
    mon_ui->graphicsView->scale(qreal(1), qreal(1));

    //QPixmap afrique("./map/afrique.JPG");

    //QPixmap afrique("./map/marrakech.JPG");
   // ma_scene->setBackgroundBrush(afrique);
    ma_scene->setSceneRect(0 ,0, 5000, 5000);






    setMinimumSize(400, 600);
    setWindowTitle(tr("Graph Creator"));
}

MainWindow::~MainWindow()
{
    delete mon_graphe;
    delete ma_scene;
    delete mon_ui;
}

void MainWindow::addNode(QPointF pos)
{

    int value = mon_graphe->numberOfNodes() + 1;
    sommet* newNode2 = new sommet(value);
    newNode2->Mode=currentMode;
    QObject::connect(newNode2, SIGNAL(activated(sommet*)),   this, SLOT(nodeActivated(sommet*)));
    QObject::connect(newNode2, SIGNAL(deactivated(sommet*)), this, SLOT(nodeDeactivated(sommet*)));
    ma_scene->addItem(newNode2);
    mon_graphe->addNode(newNode2);
    //newNode2->setPos(-50,10);

    newNode2->setPos(pos);
}

void MainWindow::nodeDeactivated(sommet *node)
{
    if(*from == *node)  {from = to;}
    to = nullptr;
    Active--;
}

void MainWindow::nodeActivated(sommet *node)
{
    if(!Active)
    {
        from = node;
        Active++;
    }
    else
    {
        to = node;
        if (from->edgeTo(to) == nullptr)
        {
            int q = QMessageBox::question(this, "ajouter arete", "voulez vous ajouter une arete reliant ces deux sommets", QMessageBox::Yes | QMessageBox::No);
            if (q == QMessageBox::Yes)
            {
                bool ok = false;
                int j=0,k=0;
                for(k=0;k<from->voisins.size();k++)
                {
                    if((from->voisins)[k]==to) j=1;
                }
                if(j==0)
                {
                    int poid=QInputDialog::getInt(nullptr,"poids","Quel est le poids?",10,0,2000,1,&ok);
                    if(ok){
                    arete *aret=new arete(from,to,poid);
                    ma_scene->addItem(aret);
                    from->voisins.push_back(to);
                    to->voisins.push_back(from);
                    mon_graphe->Edges.push_back(aret);
                    mon_graphe->Pgraph.push_back(aret);}
                    startAdding=false;
                    mon_ui->pushButton_5->setText("Debuter l'ajout de pins");

                }
                else
                {
                    QMessageBox::critical(nullptr, "DANGER", "ARETE DEJA EXISTANTE");
                }
            }
        }

        Active++;
        from->deactivate();
        to->deactivate();
        from = to = nullptr;
        Active = 0;
    }
}

sommet* MainWindow::getNodeById(int id,GraphWidget *graph )
{
    foreach (sommet* node, graph->Nodes())
    {
        if(node->getId()==id) return node;
    }
    return nullptr;
}
QVector<arete *> MainWindow::SortedEdges(QVector<arete *> SortEdges){

    arete *o;
    int i,j;
    /*for(i=0;i< Sortmon_graphe->Edges.size();i++){
        // Sortmon_graphe->Edges.push_back(new arete(Sortmon_graphe->Edges[i]->getSrc(),Sortmon_graphe->Edges[i]->getDest(),Sortmon_graphe->Edges[i]->getPoid()));
      Sortmon_graphe->Edges.push_back(mon_graphe->Edges[i]);
    }*/
    for(i=0;i<SortEdges.size();i++){
        for(j=i+1;j<SortEdges.size();j++){
            if(SortEdges[i]->poid>SortEdges[j]->poid){
                o=SortEdges[i];
                SortEdges[i]=SortEdges[j];
                SortEdges[j]=o;
            }
        }
    }
    qInfo("sort edge exec");
    return SortEdges;
}


void MainWindow::deleteSommet(int id){

    int i,j,idd=id;
    if(id<mon_graphe->Nodes().size()+1)
    {
        sommet* node=getNodeById(id,mon_graphe);
        for(i=0;i<node->voisins.size();i++)
        {
            for(j=0;j<node->voisins[i]->voisins.size();j++)
            {
                if(node->voisins[i]->voisins[j]==node) node->voisins[i]->voisins.remove(j);
            }
            for(j=0;j<node->voisins[i]->listeAretes.size();j++)
            {
                if((node->voisins[i]->listeAretes[j]->getSrc()==node)||(node->voisins[i]->listeAretes[j]->getDest()==node))
                {
                    node->voisins[i]->listeAretes.remove(j);
                }
            }
        }
        for(i=0;i<node->listeAretes.size();i++)
        {
            if((node==node->listeAretes[i]->getSrc() || node==node->listeAretes[i]->getDest()) && node->listeAretes.size()!=0)
                for(j=0;j<mon_graphe->Edges.size();j++){
                    if(mon_graphe->Edges[j]==node->listeAretes[i]) mon_graphe->Edges.remove(j);
                }
            node->edges().remove(i);
            ma_scene->removeItem(node->listeAretes[i]);

        }

        mon_graphe->ma_listeAdjacence.erase(node);
        node->listeAretes.clear();
        node->voisins.clear();
        node->edges().clear();

        int b=mon_graphe->Nodes().size();

        for ( idd ; idd <b+1 ; idd++)
        {
            mon_graphe->Nodes()[idd-1]->id--;
            mon_graphe->Nodes()[idd-1]->update();
        }

        delete node;

        node=nullptr;
    }
}


void MainWindow::randomgraph(int nbrnodes=0, int nbrEdges=0,int maxweight=0)
{
    newGraph();
    int id1;
    int id2;
    for(int v=0;v<nbrnodes;v++)
    {
        int value = mon_graphe->numberOfNodes() + 1;
        sommet* newNode = new sommet(value);
        newNode->Mode=currentMode;
        int pos1 =  (rand() %  (mon_ui->graphicsView->width()-100) + (100) );
        int pos2 = (rand() %  (mon_ui->graphicsView->height()-100)+ (100) );


        newNode->setPos(pos1,pos2);
        QObject::connect(newNode, SIGNAL(activated(sommet*)),   this, SLOT(nodeActivated(sommet*)));
        QObject::connect(newNode, SIGNAL(deactivated(sommet*)), this, SLOT(nodeDeactivated(sommet*)));

        ma_scene->addItem(newNode);
        mon_graphe->addNode(newNode);

    }

    if(nbrEdges>(nbrnodes*nbrnodes-(nbrnodes*(1+nbrnodes)/2)))
    {
        nbrEdges=nbrnodes*nbrnodes-(nbrnodes*(1+nbrnodes)/2);
    }
    if(nbrEdges!=0){
        int nombreAreteCreer=nbrEdges;

        do
        {
            do
            {
                id1 = rand() % (nbrnodes) + 1;
                id2 = rand() % (nbrnodes) + 1;
            } while( id1==id2 );
            int kiloVar = 0;
            if(maxweight!=0)
            {
                kiloVar = rand() % (maxweight) + 0 ; }

            int j=0,k=0;

            for(k=0;k<getNodeById(id1,mon_graphe)->voisins.size();k++)
            {
                if((getNodeById(id1,mon_graphe)->voisins)[k]==getNodeById(id2,mon_graphe)) j=1;
            }
            if(j==0)
            {
                nombreAreteCreer--;
                arete* a=new arete(getNodeById(id1,mon_graphe),getNodeById(id2,mon_graphe),kiloVar);
                ma_scene->addItem(a);
                mon_graphe->Edges.push_back(a);
                mon_graphe->Pgraph.push_back(a);
                getNodeById(id1,mon_graphe)->voisins.push_back(getNodeById(id2,mon_graphe));
                getNodeById(id2,mon_graphe)->voisins.push_back(getNodeById(id1,mon_graphe));

            }
        } while( nombreAreteCreer!=0 );}
}

void MainWindow::newGraph()
{
    ma_scene->clear();
    mon_graphe->Edges.clear();
    mon_graphe->Pgraph.clear();
    mon_ui->graphicsView->viewport()->update();
    delete mon_graphe;
    mon_graphe = new GraphWidget(ma_scene);
    from = to = nullptr;
    Active = 0;
}

void MainWindow::on_pushButton_clicked()
{
    randomgraph(arg1,arg2,arg3);
}
void MainWindow::on_pushButton_5_clicked()
{
    startAdding=!startAdding;
    if(startAdding==true){
        mon_ui->pushButton_5->setText("ARRETER l'ajout de pins");

    }
    if(startAdding==false){
        mon_ui->pushButton_5->setText("Debuter l'ajout de pins");
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    newGraph();
}

void MainWindow::on_spinBox_2_valueChanged(int arg33)
{
    arg3=arg33;
}

void MainWindow::on_spinBox_3_valueChanged(int arg22)
{
    arg2=arg22;
}

void MainWindow::on_spinBox_valueChanged(int arg11)
{
    arg1=arg11;
}

void MainWindow::on_pushButton_6_clicked()
{
    bool ok = false;
    int id = QInputDialog::getInt(nullptr, "sommets", "id de sommet a suprimer :", 1, 1,mon_graphe->Nodes().size(),1, &ok);
    if(ok){
    deleteSommet(id);}

}
/*#if QT_CONFIG(wheelevent)
    void MainWindow::wheelEvent(QWheelEvent *event)
    {
        //scale(pow(2., -event ->angleDelta().y() / 240.0));;
    }
#endif*/

void MainWindow::scale(qreal scaleFactor)
{
    qreal factor = mon_ui->graphicsView->QGraphicsView::transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100) return;

    mon_ui->graphicsView-> scale(scaleFactor, scaleFactor);
}

void MainWindow::zoomIn()
{
    scale(qreal(1.2));
}

void MainWindow::zoomOut()
{
    scale(1 / qreal(1.2));
}
int meil=0;


void MainWindow::on_pushButton_4_clicked()
{
    updateAretes();
    if(mon_graphe->Nodes().size()==0){
        QMessageBox::critical(nullptr, "DANGER", "Veuillez dessiner un graphe!");}
    else{

    decolorier();
    k_ArbreCouvrant(mon_graphe->Nodes().size()-1);
    QMessageBox::information(nullptr,tr("son poids est"),QString::number(meil));
    int q = QMessageBox::question(this, "suprimez les autres aretes", "voulez vous suprimez les autres aretes?", QMessageBox::Yes | QMessageBox::No);
    if (q == QMessageBox::Yes)
    {
        for(int i=0;i<mon_graphe->Edges.size();i++){
            if(mon_graphe->Edges[i]->coloring==false){
                ma_scene->removeItem(mon_graphe->Edges[i]);

            }
        }}



    }
}

void MainWindow::on_pushButton_7_clicked()
{
 ids(mon_graphe->Edges);
}

//mn hna badya load ou save
bool MainWindow::saveConfirmation()
{
    QString warning = QString("Le graphe a été modifié.\n Voulez vous enregistrer les modifications?");
    int respond = QMessageBox::warning(this, tr("Enregistrer"), warning,
                                       QMessageBox::Yes | QMessageBox::No |
                                       QMessageBox::Cancel);
    if (respond == QMessageBox::Yes)
        return save();
    else if (respond == QMessageBox::Cancel)
        return false;
    return true;
}

void MainWindow::open()
{
    if (saveConfirmation())
    {
        QString loadFileName = QFileDialog::getOpenFileName(this,
                                                            tr("Loading existing graph"),
                                                            "",
                                                            tr("Graph (*.sfn)"));
        if (!loadFileName.isEmpty())
        {
            loadGraphFromFile(loadFileName);
            file = loadFileName;

        }
    }
}

bool MainWindow::save()
{
    if (file.isEmpty())
        return saveAs();
    else
        return saveGraphToFile(file);
}

bool MainWindow::saveAs()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save current graph"),
                                                        "new_graph.sfn",
                                                        tr("Graph (*.sfn)"));
    if (!saveFileName.isEmpty())
    {
        file = saveFileName;
        return saveGraphToFile(saveFileName);
    }
    else
        return false;
}


bool MainWindow::saveGraphToFile(const QString& saveFileName)
{

    int sizeNodes = mon_graphe->Nodes().size();
    int sizeEdges = mon_graphe->Edges.size();
    QFile file(saveFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("graph");
    stream.writeStartElement("info");
    QXmlStreamAttributes attribs;
    attribs.append("sizeNodes",QString::number(mon_graphe->Nodes().size()));
    attribs.append("sizeEdges",QString::number(mon_graphe->Edges.size()));
    stream.writeAttributes(attribs);
    stream.writeEndElement();

    // nodes
    for(int i = 0 ; i < sizeNodes ; i++)
    {
        stream.writeStartElement("node");
        QXmlStreamAttributes attribs;
        attribs.append("x", QString::number(mon_graphe->Nodes()[i]->pos().x()));
        attribs.append("y", QString::number(mon_graphe->Nodes()[i]->pos().y()));
        attribs.append("index", QString::number(mon_graphe->Nodes()[i]->getId()));
        stream.writeAttributes(attribs);
        stream.writeEndElement();
    }
    //mon_graphe->Edges
    for(int j=0 ; j < sizeEdges ; j++)
    {
        stream.writeStartElement("edge");
        QXmlStreamAttributes attribs;
        attribs.append("begin", QString::number(mon_graphe->Edges[j]->getSrc()->getId()));
        attribs.append("end", QString::number(mon_graphe->Edges[j]->getDest()->getId()));
        attribs.append("weight", QString::number(mon_graphe->Edges[j]->getPoid()));
        stream.writeAttributes(attribs);
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();
    file.close();
    return true;
}



bool MainWindow::loadGraphFromFile(const QString& loadFileName)
{
    int sizeNodes;
    newGraph();

    QFile file(loadFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QXmlStreamReader stream(&file);

    while (!stream.atEnd() && !stream.hasError())
    {
        QXmlStreamReader::TokenType token = stream.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {

            if (stream.name() == "info")
            {
                QXmlStreamAttributes attribs = stream.attributes();
                sizeNodes = attribs.value("sizeNodes").toInt();
            }
            if (stream.name() == "node")
            {
                QXmlStreamAttributes attribs = stream.attributes();

                QPoint pos(attribs.value("x").toInt(), attribs.value("y").toInt());
                int ID= attribs.value("index").toInt();

                createNode(pos, ID);
            }
            if (stream.name() == "edge")
            {
                QXmlStreamAttributes attribs = stream.attributes();

                int beginNodeIndex = attribs.value("begin").toInt();
                int endNodeIndex = attribs.value("end").toInt();
                int weight = attribs.value("weight").toInt();

                createEdge(beginNodeIndex, endNodeIndex, weight,sizeNodes);
            }
        }
    }
    file.close();
    return true;
}


void MainWindow::createNode(const QPoint& pos,int idtf)
{

    sommet* newNode = new sommet(idtf);
    newNode->setPos(pos);
    QObject::connect(newNode, SIGNAL(activated(sommet*)), this, SLOT(nodeActivated(sommet*)));
    QObject::connect(newNode, SIGNAL(deactivated(sommet*)), this, SLOT(nodeActivated(sommet*)));
    ma_scene->addItem(newNode);
    mon_graphe->addNode(newNode);
}

void MainWindow::createEdge(int firstNodeIndex, int secondNodeIndex, int weight, int sizeNodes)
{
    //int sizeNodes = mon_graphe->Nodes().size();

    sommet *source = new sommet();
    sommet *destination = new sommet();


    for(int j= 0 ; j < sizeNodes ; j++)
    {
        if(mon_graphe->Nodes()[j]->getId() == firstNodeIndex)
            source = mon_graphe->Nodes()[j];
        if(mon_graphe->Nodes()[j]->getId() == secondNodeIndex)
            destination = mon_graphe->Nodes()[j];
    }

    arete *newEdge = new arete(source,destination,weight);
    source->voisins.push_back(destination);
    destination->voisins.push_back(source);
    ma_scene->addItem(newEdge);
    mon_graphe->addEdge(newEdge);
    mon_graphe->Edges.push_back(newEdge);
    mon_graphe->Pgraph.push_back(newEdge);
   // Edges.push_back(newEdge);
    //Pgraph.push_back(newEdge);
}

void MainWindow::on_actionOuvrir_triggered()
{
    open();
}

void MainWindow::on_actionEnregistrer_triggered()
{
    save();
}
//end dial load ou save




QVector<arete *>  MainWindow::Areteslie(sommet* som,QVector<arete *> copieofGraph){
    QVector<arete *> lista;


    for(int i=0;i<copieofGraph.size();i++){
        if(som==copieofGraph[i]->getSrc() || som==copieofGraph[i]->getDest()){
            lista.push_back(copieofGraph[i]);
        }
    }
    return lista;
}
void MainWindow::decolorier(){
    for(int i=0;i<mon_graphe->Edges.size();i++){
        mon_graphe->Edges[i]->coloring=false;
        mon_graphe->Edges[i]->update();

    }
}
void MainWindow::updateAretes(){
    int b=mon_graphe->Edges.size();
    for(int i=0;i<b;i++){
        if(mon_graphe->Edges[i]->suprim==true){
            mon_graphe->Edges.removeAt(i);
            i--;
            b--;
        }
    }
    mon_graphe->Pgraph=mon_graphe->Edges;
}

bool MainWindow::cyclique(QVector<sommet*> node, QVector<arete *> grouparete){
    int i=0,j;
    for(i=0;i<node.size();i++){
        node[i]->vis=0;
    }
    QVector<sommet *> stack;
    QVector<sommet *> visited;
    sommet *current;
    QVector<arete*>  groupearetecurrent;
    QVector<sommet *> voisinCurent;


    stack.push_back(node[0]);


    while(stack.size()!=0){
        current=stack[0];
        stack.remove(0);
        groupearetecurrent.clear();
        groupearetecurrent=Areteslie(current,grouparete);
        voisinCurent.clear();
        //qInfo()<<"node current est "<<current->getId();
        // remplir voisinscurrent des voisin de ce sommet dans ce groupement darrete ; possibilité davoir le sommet etudié plusieur fois dans ce vector
        for(int p=0;p<groupearetecurrent.size();p++){
            if(groupearetecurrent[p]->getSrc()->getId()==current->getId()){
                voisinCurent.push_back(groupearetecurrent[p]->getDest());
                // qInfo()<<"on a bien ajouté le sommet numer "<<groupearetecurrent[p]->getDest()->getId();
            }
            if(groupearetecurrent[p]->getDest()->getId()==current->getId()){
                voisinCurent.push_back(groupearetecurrent[p]->getSrc());
                //  qInfo()<<"on a bien ajouté le sommet numer "<<groupearetecurrent[p]->getSrc()->getId();

            }
            // suprimer le sommet current de ce vector
            //  for(int k=0;k<voisinCurent.size();k++){
            //    if(voisinCurent[k]->getId()==current->getId()){
            //      qInfo()<<"voisins current size "<<voisinCurent.size();
            //    voisinCurent.remove(k);

            //}


            // }
        }
        //for(int ok=0;ok<voisinCurent.size();ok++){


        // qInfo()<<"les id voisin de " << current->getId()<<" sont "<<voisinCurent[ok]->getId();  }


        for(i=0;i<voisinCurent.size();i++){
            if(voisinCurent[i]->vis!=1) stack.push_back(voisinCurent[i]);
        }
        visited.push_back(current);
        current->vis=1;

    }
    for(i=0;i<visited.size();i++){
        for(j=i+1;j<visited.size();j++){
            if(visited[i]==visited[j]) return true;
        }
    }
    return false;

}
void MainWindow::on_pushButton_3_clicked()
{
    updateAretes();
    if(mon_graphe->Nodes().size()==0){
        QMessageBox::critical(nullptr, "DANGER", "Veuillez dessiner un graphe!");}
    else{
    decolorier();
    if(karbre>mon_graphe->numberOfNodes()-1){
        QMessageBox::information(nullptr,tr("info"),"veuillez donner un k inferieur au nombre de sommets");
    }
    else{
        k_ArbreCouvrant(karbre);
         QMessageBox::information(nullptr,tr("son poids est"),QString::number(meil));
        int q = QMessageBox::question(this, "suprimez les autres aretes", "voulez vous suprimez les autres aretes?", QMessageBox::Yes | QMessageBox::No);
        if (q == QMessageBox::Yes)
        {
            for(int i=0;i<mon_graphe->Edges.size();i++){
                if(mon_graphe->Edges[i]->coloring==false){
                    ma_scene->removeItem(mon_graphe->Edges[i]);

                }
            }}
    }
    }}
    //else{
      //   QMessageBox::critical(nullptr, "DANGER", "Graphe non connexe!");
    //}
//}
//QVector<arete*> mon_graphe->Pgraph;


QVector<sommet*> MainWindow::ids(QVector<arete*> graph ){
    QVector<sommet*> lesSommetDeGraph;
    for(int i=0;i<graph.size();i++){
        lesSommetDeGraph.push_back(graph[i]->src);
        lesSommetDeGraph.push_back(graph[i]->dest);
    }
    for(int s=0;s<lesSommetDeGraph.size();s++){
        for(int h=0;h<lesSommetDeGraph.size();h++){
            if(h!=s){
                if(lesSommetDeGraph[s]->getId()==lesSommetDeGraph[h]->getId()){
                    lesSommetDeGraph.remove(h);
                    if(s==lesSommetDeGraph.size()){
                        break;
                    }
                }}
        }
    }


    return lesSommetDeGraph;

}
int begin=0;
bool stop=false;
QVector<arete*> MainWindow::ArbreCouvrant( int k)
{
    qInfo()<<"***************************BEGIN*****************";

    mon_graphe->Pgraph=SortedEdges(mon_graphe->Pgraph);

 int a=0;//un int qu sincremente jusqua K ie jusqua la collecte de les k aretes
    QVector<arete*> primOne;
    QVector<arete*> Pvoisin;//voisin des sommets sources
    QVector<arete*> Pvoisin1;// voisin des sommets dest
    QVector<arete*> primTwo;//le resultats final
    int Currentpoid=0;// poids actuel du k arbre qu'on construit
   // QVector<sommet *> sommetPrimOneFirst;
    qInfo()<<"begin est egal a "<<begin;
    qInfo()<<"mon_graphe->Pgraph size  "<<mon_graphe->Pgraph.size();
    primTwo.push_back(mon_graphe->Pgraph[begin]);// larete du commencement
    Currentpoid=primTwo[0]->getPoid();// le poids de la premiere arete dans primTwo
    if(Currentpoid>meil){// compare le poids de cette arete avec le poids du meilleur one si plus grand pas la peine de chercher sans k arbre alors on sort
        mon_graphe->Pgraph.remove(begin);
        mon_graphe->Pgraph=mon_graphe->Edges;
        stop=true;
        QVector<arete*> sizenull;

        qInfo()<<"size null est sortie ";
        return sizenull;
    }
    qInfo()<<"after the condition ";
    mon_graphe->Pgraph.remove(begin);// suppression de la premiere arete du resultat dans le pgraph
    if(k==1){return primTwo;}// si le k=1 c a d return la plus petite arete du graphe
    bool cycle=false;// bool pour savoir si cycle ou non




    do{// boucle qui construit le k arbre
        for(int i=0;i<primTwo.size();i++){// collecte des aretes voisins

            Pvoisin1=Areteslie(primTwo[i]->getSrc(), mon_graphe->Pgraph);

            Pvoisin=Areteslie(primTwo[i]->getDest(),mon_graphe->Pgraph);


            for(int c=0;c<Pvoisin.size();c++){// des conditions pour ne pas ajouter les meme aretes plusieurs fois

                int chek1=0;
                for(int lop=0;lop<primOne.size();lop++){
                    if(primOne[lop]==Pvoisin[c])
                        chek1=1;  }

                if(chek1==0){
                    primOne.push_back(Pvoisin[c]);// ajout des voisins src dans le primOne PS: primOne et une liste qui contiendra les voisin src et dest en meme temps cad Pvoisin+Pvoisin1 sans ajout des meme aretes = PrimOne

                }}
            for(int c=0;c<Pvoisin1.size();c++){

                int chek2=0;
                for(int lop=0;lop<primOne.size();lop++){
                    if(primOne[lop]==Pvoisin1[c])
                        chek2=1;  }

                if(chek2==0){
                    primOne.push_back(Pvoisin1[c]);// same thing ajout des voisin dest dans primOne

                }}

        }
        if(primOne.size()!=0){
            primOne=SortedEdges(primOne);}// trié primOne
        int n=primOne.size()+1;
        do{

            if(primOne.size()!=0){
                    //nice idea

                   // sommetPrimOneFirst.push_back(primOne.first()->getSrc());
                    //sommetPrimOneFirst.push_back(primOne.first()->getDest());
                    cycle=false;
                    /*for(int i=0;i<sommetDePrimtwo.size();i++){
                        if(cycle==true) break;
                      if(sommetDePrimtwo[i]==sommetPrimOneFirst[0]){
                          for(int i=0;i<sommetDePrimtwo.size();i++){
                              if(sommetDePrimtwo[i]==sommetPrimOneFirst[1]){
                                  cycle=true;
                                  sommetPrimOneFirst.clear();
                                  break;
                                  }
                          }
                      }
                    }*/
                    primTwo.push_back(primOne.first());// on ajout la plus petite arete dans primTwo
                     QVector<sommet *> sommetDePrimtwo=ids(primTwo);// on collecte les sommet de primTwo
                    if(primTwo.size()!=sommetDePrimtwo.size()-1){//condition proposé par bouguessa pour savoir si cycle ou non
                        cycle=true;
                    }
                if(cycle==true){// si cycle true
                       primTwo.removeLast();// first of all on suprimer la derniere arete qu'on vien dajouté dans primTwo
                    for(int k=0;k<mon_graphe->Pgraph.size();k++){// on cherche cette aretes dans le Pgraph et on la supprime aussi
                        if(primOne[0]==mon_graphe->Pgraph[k]) {
                            mon_graphe->Pgraph.remove(k);
                            qInfo("removed arete faisant un cycle from lgraph lkbir");
                            break;
                        }
                    }

                    primOne.removeFirst();// on supprimera cette arete dans primOne pour passer au next one
                    n--;

                }
            }
            if(primOne.size()==0){
                break;
            }
        }while(cycle==true && primOne.size()!=n );// boucle a repeter jusqua avoir cycle false


        if(primOne.size()!=0 ){
            //primTwo.push_back(primOne[0]);
            Currentpoid+=primOne[0]->getPoid();// si le compilateur est ici c a d que le cycle est false alors on garde l'arrete qu'on ajouté et on ajoute son poids dans la variable currentpoid
            qInfo()<<"primTwo size is "<<primTwo.size();
            primOne.clear();// on vide primOne

            if(Currentpoid>meil){// toujours on compare si le poids actuel est superieur au poids total du meilleur one si superieur on STOP
                mon_graphe->Pgraph=mon_graphe->Edges;
                QVector<arete*> sizenull;
                qInfo()<<"size null est sortie ";
                return sizenull;
            }

        }
        Pvoisin.clear();// vider Pvoisin car on aura dautre voisin
        Pvoisin1.clear();// sameThing
        //sommetPrimOneFirst.clear();

        a++;// la variable de nombre de fois a bouclé on ajouté une arete donc on incremente
        for(int k=0;k<mon_graphe->Pgraph.size();k++){// on boucle est on cherche l'arete qu'on vient dajouté a primTwo , on la supprime dans le Pgraph
            if(primTwo[primTwo.size()-1]==mon_graphe->Pgraph[k]) {
                mon_graphe->Pgraph.remove(k);
                qInfo("removed arete from lgraph lkbir");
                break;
            }}

        qInfo()<<"mon_graphe->Pgraph size is "<<mon_graphe->Pgraph.size();

    }while(a<k-1);// boucle a repeté jusqua avoir le K arete
    mon_graphe->Pgraph=mon_graphe->Edges;// remetre Pgraph a la normale
    return primTwo;// resultat
}

void MainWindow::k_ArbreCouvrant(int k=5){
    meil=10000000;
    //mon_graphe->Pgraph=SortedEdges(mon_graphe->Edges);
    int poi=0;
    long long c=100000000000000000;
    int sfn;
    int i=0;
    //int n=mon_graphe->Pgraph.size();
    QVector<arete*> resultat;// current resultat pour comparé le poids avec meilleurOne
    QVector<arete*> meilleurOne;// le meilleur resultat so far
    bool ontoThenextOne=true;//bool qui sert a dire que le current resultat a pour linstant un poids meilleur (inferieur du meilleur one)
    int oki=mon_graphe->Pgraph.size();// le nombre de fois a bouclé puisque brute force donc on boucle sur toutes les aretes
    do{

       ontoThenextOne=true;
        poi=0;
        resultat=ArbreCouvrant(k);// execution de lautre algo pour avoir le k arbre
        if(stop==true){

            stop=false;
            break;
        }
        if(resultat.size()==0){// quand par exemple lalgo trouve 2 arete deja leur poids est superieur au meilleur one il envoie une liste vide bach ana ngol lhad lalgo ila kant liste vide ngolih bli tanta bla matkml lexecution w doz l next arete
           ontoThenextOne=false;
        }
        if(k==1){// ila kan k =1 rah meilleur one aylqah f la premiere itération y3ni la plus petite arete
            meilleurOne=resultat;
            break;
        }
        if(k==mon_graphe->Nodes().size()-1){// hna ila kan k howa nombre de sommet -1 y3ni probleme de larbre couvrant y3ni makaynch lach tboucli 3la les autres arete hint il ya un est un seul arbre couvrant 'de poids minimal' maymknch ylqa wahed w mnb3d ylqa mahsen mno
            meilleurOne=resultat;
            break;
        }
        oki--;
        begin++;// begin howa lindice mnin kaybda lalgo lakhor Pgraph[begin]
        if(resultat.size()==k && ontoThenextOne){// hna ila makanch probleme de larbre couvrant w makanch k howa 1 w kan le poids mzyaan mn lmeilleur one
            for(int g=0;g<resultat.size();g++){// hadchi hir boucle bach ytafficha lia l3ibat flconsole
                poi+=resultat[g]->getPoid();}
            qInfo("**************RESULTAT**************");
            for (int i=0;i<resultat.size();i++){
                qInfo()<<"poid min arrete resultat "<<i<<" = "<<resultat[i]->poid<<" dest id : "
                      <<resultat[i]->getDest()->getId()<<" Src Id : "<<resultat[i]->getSrc()->getId();
            }
            sfn=poi;// sfn variable kaykon fiha l current poid
            qInfo()<<"le Poids total de ce resultat est "<<poi;

            if(sfn<c){// c variable dial lpoid precedent bach t9arn current poid m3a lprecedent
                qInfo()<<"c EST egal a"<<c;
                c=sfn;// C kaywli fiha l current poid bach fach ndozo lnext indice aykon fiha techniquement le poids precedent dial dak lindice
                meilleurOne=resultat;// hna kanstokiw had l resultat aywli howa lmeilleur one hintach le poids dialo sgher mn C w btw C rah fiha dima lmeilleur poid tahia i guess
                i++;// hir bach n3rf dkche f console ch7al mn meilleur one lqina
                qInfo()<<"j'ai trouvé un meilleur k arbre "<<i;
                meil=0;
                for (int i=0;i<meilleurOne.size();i++){// kanstokiw le poids f had meil lihia variable local w kanst3mlha f lalgo lakhor //// hna bant lia wahed l3ba bla manb9a nhsb waqila ha ndir meil = c wla la ? saraha tanchuf ma3rftch

                    meil+=meilleurOne[i]->getPoid();

                }
            }

        }
    }while (oki!=0);// sf hna kanboucliw taywli dak oki = 0 z3ma rah boucla 3la toutes les aretes



    // meilleurOne=resultat;
    // meilleurOne=ArbreCouvrant(mon_graphe->Pgraph,k);
    qInfo("*****************MEILLEURONE*******************");
    //poi=0;
     meil=0;
    for (int i=0;i<meilleurOne.size();i++){
        qInfo()<<"poid min arrete meilleurOne "<<i<<" = "<<meilleurOne[i]->poid<<" dest id : "<<meilleurOne[i]->getDest()->getId()<<" Src Id : "<<meilleurOne[i]->getSrc()->getId();
    // hadchi limcommenti lt7t howa likaydir dik visualisation
        /*QTimer leTemp;
        QEventLoop leFor;

        leTemp.setInterval(500);
        QObject::connect(&leTemp, &QTimer::timeout, &leFor, &QEventLoop::quit);
        leTemp.start();
        leFor.exec();*/
        meilleurOne[i]->coloring=true;
        update();

       meil+=meilleurOne[i]->getPoid();

    }
    begin=0;// sf mora masalina khassna nrdo dkche kikan flwl begin yrj3 lzero
    mon_graphe->Pgraph=mon_graphe->Edges;// w Pgraph yrj3 kikan = edges
    qInfo()<<"le Poids total de ce MEILLEUR est "<<meil;

    // mon_graphe->Pgraph.clear();
    // meilleurOne=ArbreCouvrant(mon_graphe->Pgraph,k);
}










void MainWindow::on_pushButton_8_clicked()
{

}






void MainWindow::on_pushButton_9_clicked()
{
    updateAretes();
    if(mon_graphe->Nodes().size()==0){
        QMessageBox::critical(nullptr, "DANGER", "Veuillez dessiner un graphe!");}
    else{
    bool a=mon_graphe->connexe();
    if(a==false){
        QMessageBox::critical(nullptr, "DANGER", "je vous deconseille d'executer le k_arbre car ce graph n'est pas connexe");}
    if(a==true){
        QMessageBox::information(nullptr,tr("info"),"ce graphe est connexe vous pouvez executé le k_arbre");
    }}
}

void MainWindow::on_pushButton_11_clicked()
{
    zoomIn();
}

void MainWindow::on_pushButton_10_clicked()
{
    zoomOut();
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if(startAdding){
        QPoint origin = mon_ui->graphicsView->mapFromGlobal(QCursor::pos());
        //QPoint(12,-6);
        QPointF relativeOrigin = mon_ui->graphicsView->mapToScene(origin);

        addNode(relativeOrigin);
       // mon_graphe->Nodes()[mon_graphe->Nodes().size()-1]->cursor=relativeOrigin;
        update();}
}

void MainWindow::on_actionMarrakech_triggered()
{

    QPixmap marrakech("./map/marrakech.jpg");

    ma_scene->setBackgroundBrush(marrakech);
    //ma_scene->setSceneRect(0,0,5000,5000);

    update();
}

void MainWindow::on_actionMedina_triggered()
{
    QPixmap medina("./map/medina.jpg");
    ma_scene->setBackgroundBrush(medina);
    //ma_scene->setSceneRect(0,0,5000,5000);

    update();
}

void MainWindow::on_actionAfrique_triggered()
{
    QPixmap afrique("./map/afrique.JPG");
    ma_scene->setBackgroundBrush(afrique);
     //ma_scene->setSceneRect(0,0,5000,5000);

    update();
}

void MainWindow::on_actionaucune_triggered()
{
    ma_scene->setBackgroundBrush(Qt::white);
   // ma_scene->setSceneRect(0,0,5000,5000);


    update();
}

void MainWindow::on_actionMa_propre_map_image_triggered()
{
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir Ma propre map", QString(), "Images (*.PNG *.gif *.jpg *.jpeg *.bmp *.svg");
    QPixmap selected(fichier);
    ma_scene->setBackgroundBrush(selected);
     ma_scene->setSceneRect(0,0,5000,5000);
}

void MainWindow::on_actionSwitch_Mode_triggered()
{
    for(int i=0;i<mon_graphe->Nodes().size();i++){
        mon_graphe->Nodes()[i]->Mode=!mon_graphe->Nodes()[i]->Mode;

    }
    currentMode=!currentMode;
    update();

}

void MainWindow::on_spinBox_4_valueChanged(int arg1)
{
    karbre=arg1;
}
