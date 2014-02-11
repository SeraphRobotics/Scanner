#include "scannerfunctions.h"
#include <stdio.h>
#include "math/float.h"
#include <QDomDocument>
#include <math.h>
#include "shared/stl/stlfile.h"


bool onLoops(FAHVector3 pt, const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops){
    bool onouter=false;
    bool oninner=false;
    foreach(FAHLoopInXYPlane loop,innerLoops){
        oninner=loop.pointOnLoop(pt);
        if (oninner){
            return oninner;
        }
    }

    onouter = OuterLoop.pointOnLoop(pt);
    return onouter;

}

bool loopsContain(FAHVector3 pt, const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops){
    bool containedByOuter=true;
    bool notContainedByInners=true;
    foreach(FAHLoopInXYPlane loop,innerLoops){
        notContainedByInners = notContainedByInners && !loop.pointInside(pt);
    }

    containedByOuter = OuterLoop.pointInside(pt);
    return (containedByOuter&&notContainedByInners);
}

void printPoint(FAHVector3 pt){
    printf("\n\t%.2f\t%.2f\t%.2f",pt.x,pt.y,pt.z);
}

void writeLoopToXDFL(const FAHLoopInXYPlane& loop, QString file){
     QDomDocument document;
     QDomElement root = document.createElement("xdfl");
     document.appendChild(root);

     /// ADD palette
     QDomElement palette = document.createElement("palette");
     root.appendChild(palette);
     QDomElement name = document.createElement("name");
     name.appendChild(document.createTextNode("test"));
     palette.appendChild(name);
     QDomElement id = document.createElement("id");
     id.appendChild(document.createTextNode("1"));
     palette.appendChild(id);

     QDomElement cmds = document.createElement("commands");
     QDomElement path = document.createElement("path");
     QDomElement matId = document.createElement("materialid");
     matId.appendChild(document.createTextNode("1"));
     path.appendChild(matId);
     for(int i=0;i<loop.points.size();i++){
         FAHVector3 pt = loop.points.at(i);

         QDomElement point = document.createElement("point");
         QDomElement xEl = document.createElement("x");
         xEl.appendChild(document.createTextNode(QString::number(pt.x)));
         QDomElement yEl = document.createElement("y");
         yEl.appendChild(document.createTextNode(QString::number(pt.y)));
         QDomElement zEl = document.createElement("z");
         zEl.appendChild(document.createTextNode(QString::number(pt.z)));

         point.appendChild(xEl);
         point.appendChild(yEl);
         point.appendChild(zEl);
         path.appendChild(point);

     }
     FAHVector3 pt = loop.points.at(0);

     QDomElement point = document.createElement("point");
     QDomElement xEl = document.createElement("x");
     xEl.appendChild(document.createTextNode(QString::number(pt.x)));
     QDomElement yEl = document.createElement("y");
     yEl.appendChild(document.createTextNode(QString::number(pt.y)));
     QDomElement zEl = document.createElement("z");
     zEl.appendChild(document.createTextNode(QString::number(pt.z)));

     point.appendChild(xEl);
     point.appendChild(yEl);
     point.appendChild(zEl);
     path.appendChild(point);
     cmds.appendChild(path);
     root.appendChild(cmds);



     QFile f(file);
     if (f.open(QFile::WriteOnly)) {
         QTextStream out(&f);
         out<<document.toString();
         f.close();
     }

}

void removePointsIfInList(QVector<FAHVector3>* vector,QVector<FAHVector3>* newvector, QList<FAHVector3> points){
    QVector<FAHVector3> copy(vector->size());

    for(int i=0;i<vector->size();i++){
        copy[i]=vector->at(i).copy();
    }

    QMutableVectorIterator<FAHVector3> iter(copy);
    while(iter.hasNext()){
        FAHVector3 pt = iter.next();
        for(int i=0;i<points.size();i++){
            FAHVector3 point = points[i];
            if(point.equals(pt)){
                iter.remove();
                i=points.size();
//                printf("\n point removed");
            }
        }
    }
    newvector->clear();
    for(int i=0;i<copy.size();i++){
        newvector->append(copy[i]);
    }
}


void removedDuplicatePoints(QVector<FAHVector3>* vector){
    QList<int> points_to_remove;
    int vectorsize= vector->size();
    QVector<FAHVector3> copy(vectorsize);
    for(int i=0;i<vectorsize;i++){
        FAHVector3 pt = vector->operator [](i);
        copy[i]=pt;
        for(int j=0;j<vectorsize;j++){
            if(i!=j){
                if((vector->at(j).equals(vector->at(i)))
                        &&!points_to_remove.contains(i)
                        &&!points_to_remove.contains(j)){
                    points_to_remove.append(i);
//                    printf("\nDupplicate\t%.2f\t%.2f\t%.2f",pt.x,pt.y,pt.z);
                    copy[i].zero();
                }
            }
        }
    }

    vector->clear();
    QVectorIterator<FAHVector3> iter(copy);
    while(iter.hasNext()){
        FAHVector3 pt = iter.next();
        if(!pt.isInvalid()){
            vector->append(pt);
        }
    }


}


bool getPointsInGrid(int i,int j, float stepsize,const FAHLoopInXYPlane& outerloop ,QList<FAHLoopInXYPlane> innerLoops, QVector<FAHVector3>* pointsInGrid){
    bool pointsFromOuterLoop=false;

    FAHVector3 p1,p2,p3,p4;
    p1=vectorFromIJ(i,j,0,stepsize);
    p2=vectorFromIJ(i,j+1,0,stepsize);
    p3=vectorFromIJ(i+1,j,0,stepsize);
    p4=vectorFromIJ(i+1,j+1,0,stepsize);

    FAHLine l12(p1,p2);
    FAHLine l13(p1,p3);
    FAHLine l24(p2,p4);
    FAHLine l34(p3,p4);
    QList<FAHLine> lines;
    lines.append(l12);
    lines.append(l13);
    lines.append(l24);
    lines.append(l34);







    // get intersections along grid
    QVector<FAHVector3> edgepoints;
    bool inloop = false;
    int numLoops = 0;


    foreach(FAHLine line,lines){
        FAHVector3 pt;
        outerloop.intersect(line,&pt);
        if(!pt.isInvalid()){
            pointsFromOuterLoop = true;
            edgepoints.append(pt.copy());
            if(!inloop){
                inloop=true;
                numLoops++;
            }

        }
    }



    for(int k=0;k<innerLoops.size();k++){
        inloop= false;
        FAHLoopInXYPlane loop = innerLoops.at(k);
        foreach(FAHLine line,lines){
            FAHVector3 pt;
            loop.intersect(line,&pt);
            if(!pt.isInvalid()){
                pointsFromOuterLoop = false;
                edgepoints.append(pt.copy());
                if(!inloop){
                    inloop=true;
                    numLoops++;
                }

            }
        }

    }

    if(numLoops>1){
        printf("\n %i LOOPS",numLoops);
    }else{


        // NEED A SYSTEM FOR solving if item passes through more than 2 walls
        int size = edgepoints.size();
//        if (size!=2){
//            printf("\n Index %i,%i Found %i intersects",i,j,edgepoints.size());
//            for(int k=0;k<edgepoints.size();k++){
//                printf("\n\tPoint: %.2f\t%.2f\t%.2ff",edgepoints.at(k).x,edgepoints.at(k).y,edgepoints.at(k).z);
//            }
//        }
        if (size>=2){
            QList<FAHLoopInXYPlane> loops(innerLoops);
            loops.append(outerloop);

            QVector<FAHVector3> innerpoints;
            for(int k=0;k<loops.size();k++){
                FAHLoopInXYPlane loop = loops.at(k);
                // Collect points between
                for(int i=0;i<loop.points.size();i++){
                    FAHVector3 pt = FAHVector3(loop.points.at(i));
                    if( (pt.x>=p1.x) && (pt.x<=p3.x) &&
                        (pt.y>=p1.y) && (pt.y<=p2.y) ){
                        // inside grid
                        innerpoints.append(pt);
                    }
                }
            }

            /// NEED TO CHECK FOR CUVES BAHAVIOR

            // Merge two lists
            pointsInGrid->append(edgepoints[0]);
            for(int i=0;i<innerpoints.size();i++){
                pointsInGrid->append(innerpoints[i]);

            }
            pointsInGrid->append(edgepoints[1]);
        }
    }
    return pointsFromOuterLoop;
}



void addFacetWithNormal(FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, STLMesh* mesh, bool zpositive){
    FAHTriangle t1(p1,p2,p3);
    STLFacet s1;
    if(((t1.normal().z>0)&&zpositive)||((t1.normal().z<0)&&!zpositive)){

        s1.triangle=t1;
        s1.normal=t1.normal();
    }else{
        FAHTriangle t2(p1,p3,p2);
        s1.triangle=t2;
        s1.normal=t2.normal();
    }
    if(!s1.normal.isInvalid()){
        mesh->AddFacet(s1);
    }

}

void addFacetWithDirection(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, FAHVector3 direction){
    FAHTriangle t1(p1,p2,p3);
    STLFacet s1;
    bool used1=false;
    if( (t1.normal().dot(direction))<0){
        used1=true;
        s1.triangle=t1;
        s1.normal=t1.normal();
    }else{
        FAHTriangle t2(p1,p3,p2);
        s1.triangle=t2;
        s1.normal=t2.normal();
    }
    if(!s1.normal.isInvalid()){
        mesh->AddFacet(s1);
    }else{
        printf("\nERROR IN NORMAL\t used:%s",(used1?"1":"2"));
        printPoint(p1);
        printPoint(p2);
        printPoint(p3);
    }
}


template <class T>
void addLoopToSTL(const FAHLoopInXYPlane& loop, XYGrid<T>* grid, STLMesh* mesh , bool inner){
    FAHLoopInXYPlane newloop;
    float ds = 10;

    for(int i=1;i<loop.points.size()+1;i++){
        FAHLine line(loop.points.at(i-1),loop.points.at(i%loop.points.size()));
        newloop.add(line.a);
        bool inseg = true;
        int j=0;
        while(inseg && (j<10000)){
            j++;
            FAHVector3 ab = (line.b.copy().sub(line.a));
            ab.normalize();
            ab*=j*ds;
            FAHVector3 pt= line.a.copy()+=ab;
            if(line.pointOnSegment(pt)){
                newloop.add(pt);
//                printf("\n added point");

            }
            else{
//                printf("\n\n Segment");
//                printPoint(line.a);
//                printPoint(line.b);
//                printPoint(pt);
                inseg=false;
            }
        }

    }


    // get point values
    QVector<FAHVector3> points(newloop.points.size());
    QVector<FAHVector3> primepoints(newloop.points.size());
    for(int k=0;k<points.size();k++){
        points[k]=newloop.points.at(k).copy();
        primepoints[k]=newloop.points.at(k).copy();
        primepoints[k].z=0;
        points[k].z=getHeightAt(points[k].x,points[k].y,grid);
    }


    // Add loop to STL
    for (int i = 1; i < points.size() + 1; i++) {
        FAHVector3 p1,p2,p1p,p2p;
        p1=points[i-1];
        p2=points[i%points.size()];
        p1p=primepoints[i-1];
        p2p=primepoints[i%points.size()];

        FAHTriangle t1(p1,p2,p2p);
        FAHVector3 testpt = p1.copy().add(t1.normal());
        bool contains = loop.pointInside(testpt);
        if(!inner){
            if(contains){
                addFacetToSTL(p2p,p2,p1,mesh);
                addFacetToSTL(p1,p1p,p2p,mesh);
            }else{
                addFacetToSTL(p1,p2,p2p,mesh);
                addFacetToSTL(p2p,p1p,p1,mesh);
            }
        }else{
            if(!contains){
                addFacetToSTL(p2p,p2,p1,mesh);
                addFacetToSTL(p1,p1p,p2p,mesh);
            }else{
                addFacetToSTL(p1,p2,p2p,mesh);
                addFacetToSTL(p2p,p1p,p1,mesh);
            }
        }

    }




}


template <class T>
STLMesh* makeSTLfromScanSection(XYGrid<T>* grid, const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops){
    STLMesh* mesh= new STLMesh();
    // Construct triangles for top and bottom


    for(int j=0;j<grid->ny()-1;j++){
        for(int i=0;i<grid->nx()-1;i++){
            /** calculated vOuterLoophe 4 points and determin case
             *  p1--p2 -->i x  thesebounds
             *  | \  |
             *  p3--p4         nextbounds
             *  |
             * \|/
             *  j
             *  y
             *
             *
             **/
            FAHVector3 p1,p2,p3,p4;
            FAHVector3 p1prime,p2prime,p3prime,p4prime;
            bool b1=true,b2=true,b3=true,b4=true;
            //bool l1=false, l2=false, l3=false, l4=false;
            int numInBounds=0;
            //int numOnLoop=0;

            p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
            p1prime = FAHVector3(&p1);
            p1prime[2]=0;
            b1=loopsContain(p1,OuterLoop,innerLoops);
            //l1=onLoops(p1,OuterLoop,innerLoops);
            if(b1){numInBounds++;}
            //if(l1){numOnLoop++;}

            p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
            p2prime = FAHVector3(&p2);
            p2prime[2]=0;
            b2=loopsContain(p2,OuterLoop,innerLoops);
            //l2=onLoops(p1,OuterLoop,innerLoops);
            if(b2){numInBounds++;}
            //if(l2){numOnLoop++;}


            p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
            p3prime = FAHVector3(&p3);
            p3prime[2]=0;
            b3=loopsContain(p3,OuterLoop,innerLoops);
            //l3=onLoops(p1,OuterLoop,innerLoops);
            if(b3){numInBounds++;}
            //if(l3){numOnLoop++;}

            p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
            p4prime = FAHVector3(&p4);
            p4prime[2]=0;
            b4=loopsContain(p4,OuterLoop,innerLoops);
            //l4=onLoops(p4,OuterLoop,innerLoops);
            if(b4){numInBounds++;}
            //if(l4){numOnLoop++;}



//            printf("\n\nGrid:%i,%i\tX:%.1f - %.1f\tY:%.1f-%.1f",
//                   i,j,p1.x,p4.x,p1.y,p4.y);

//            printf("\n Case:%i \tb1:%s\tb2:%s\tb3:%s\tb4:%s",numInBounds,
//                   (b1)?"true":"false",
//                   (b2)?"true":"false",
//                   (b3)?"true":"false",
//                   (b4)?"true":"false");


            /// THere are 5 cases 0,1,2,3,4 points are in bound
            /// for case 0 do nothing


            if(4==numInBounds){
                /// Make Triangles  p1-p4-p3 and p2-p4-p1 which form the top
                addFacetToSTL(p1,p4,p3,mesh);
                addFacetToSTL(p2,p4,p1,mesh);

                /// Make Triangles p4'-p1'-p3' and p4'-p2'-p1' with Zs=0 to form bottom
                addFacetToSTL(p4prime,p1prime,p3prime,mesh);
                addFacetToSTL(p4prime,p2prime,p1prime,mesh);
            }else if(1<=numInBounds){
                // All other cases
                bool fromOuterloop = false;
                FAHVector3 direction;


                QVector<FAHVector3> allpoints;
                QVector<FAHVector3> points;

                fromOuterloop= getPointsInGrid(i,j,grid->stepSize(),OuterLoop,innerLoops,&allpoints);
                QList<FAHVector3> gridpoints;
                gridpoints.append(p1);
                gridpoints.append(p2);
                gridpoints.append(p3);
                gridpoints.append(p4);
                for(int k=0;k<allpoints.size();k++){
                    allpoints[k].z=getHeightAt(allpoints[k].x,allpoints[k].y,p1,p2,p3,p4);
                }

                /// get rid of overlap

                removePointsIfInList(&allpoints,&points,gridpoints);

                QVector<FAHVector3> primepoints(points);
                for(int k=0;k<points.size();k++){
                    primepoints[k].z=0;
                }

                QVector<FAHVector3> allprimepoints(allpoints);
                for(int k=0;k<allpoints.size();k++){
                    allprimepoints[k].z=0;
                }

                if(2<=points.size()){
                    if(3==numInBounds){
                        FAHVector3 anchorPt, anchorPtPrime;
                        FAHVector3 leftPt, leftPtPrime;
                        FAHVector3 rightPt, rightPtPrime;
                        FAHVector3 leftBorderPt, leftBorderPtPrime;
                        FAHVector3 rightBorderPt, rightBorderPtPrime;

                        if(!b1){ // anchor to p4
                            direction = p4.copy().sub(p1);
                            anchorPt = p4;
                            anchorPtPrime=p4prime;
                            leftPt=p3;
                            leftPtPrime=p3prime;
                            rightPt=p2;
                            rightPtPrime=p2prime;
                            if(Math::floatsEqual(points.first().x,leftPt.x)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else if (Math::floatsEqual(points.last().x,leftPt.x)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b2){
                            direction = p3.copy().sub(p2);
                            anchorPt = p3;
                            anchorPtPrime=p3prime;
                            leftPt=p1;
                            leftPtPrime=p1prime;
                            rightPt=p4;
                            rightPtPrime=p4prime;
                            if(Math::floatsEqual(points.first().y,leftPt.y)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().y,leftPt.y)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b3){
                            direction = p2.copy().sub(p3);
                            anchorPt = p2;
                            anchorPtPrime=p2prime;
                            leftPt=p4;
                            leftPtPrime=p4prime;
                            rightPt=p1;
                            rightPtPrime=p1prime;
                            if(Math::floatsEqual(points.first().y,leftPt.y)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().y,leftPt.y)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }else if (!b4){
                            direction = p1.copy().sub(p4);
                            anchorPt = p1;
                            anchorPtPrime=p1prime;
                            leftPt=p2;
                            leftPtPrime=p2prime;
                            rightPt=p3;
                            rightPtPrime=p3prime;
                            if(Math::floatsEqual(points.first().x,leftPt.x)){
                                leftBorderPt = points.first();
                                leftBorderPtPrime = primepoints.first();
                                rightBorderPt = points.last();
                                rightBorderPtPrime=primepoints.last();

                            }else {//if (Math::floatsEqual(points.last().x,leftPt.x)){
                                rightBorderPt = points.first();
                                rightBorderPtPrime = primepoints.first();
                                leftBorderPt = points.last();
                                leftBorderPtPrime=primepoints.last();
                            }

                        }
                        //ADD TOP AND BOTTOM
                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }
                        addFacetWithNormal(leftBorderPt,anchorPt,leftPt,mesh,true);
                        addFacetWithNormal(leftPtPrime,anchorPtPrime,leftBorderPtPrime,mesh,false);

                        addFacetWithNormal(rightBorderPt,rightPt,anchorPt,mesh,true);
                        addFacetWithNormal(anchorPtPrime,rightPtPrime,rightBorderPtPrime,mesh,false);

//                        for(int k=0;k<points.size()-1;k++){
//                            addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
//                            addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
//                        }
//                        addFacetWithDirection(leftPt,leftBorderPt,leftBorderPtPrime, mesh,direction);
//                        addFacetWithDirection(leftBorderPtPrime, leftPtPrime,leftPt, mesh,direction);

//                        addFacetWithDirection(rightPt,rightBorderPt, rightBorderPtPrime, mesh,direction);
//                        addFacetWithDirection(rightBorderPtPrime, rightPtPrime,rightPt, mesh,direction);


                    }else if(2==numInBounds){
                    /// 4 sub cases, left,right,top and bottom
                        FAHVector3 anchorPt, anchorPtPrime;
                        FAHVector3 farPt, farPtPrime;
                        FAHVector3 farPtBorder, farPtBorderPrime;

                        if(b2&&b4){// right
                            direction = p2.copy().sub(p1);
                            anchorPt=p4;
                            anchorPtPrime=p4prime;
                            farPt=p2;
                            farPtPrime=p2prime;
                            if(Math::floatsEqual(points.last().y,farPt.y)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b1&&b3){//left
                            direction = p1.copy().sub(p2);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                            farPt=p1;
                            farPtPrime=p1prime;
                            if(Math::floatsEqual(points.last().y,farPt.y)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b1&&b2){//top
                            direction = p1.copy().sub(p3);
                            anchorPt=p1;
                            anchorPtPrime=p1prime;
                            farPt=p2;
                            farPtPrime=p2prime;
                            if(Math::floatsEqual(points.last().x,farPt.x)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }

                        }else if(b3&&b4){//bottom
                            direction = p3.copy().sub(p1);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                            farPt=p4;
                            farPtPrime=p4prime;
                            if(Math::floatsEqual(points.last().x,farPt.x)){
                                farPtBorder=points.last();
                                farPtBorderPrime=primepoints.last();
                            }else{
                                farPtBorder=points.first();
                                farPtBorderPrime=primepoints.first();
                            }
                        }
                        //STLs use right hand rule to determin normal

                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }
                        addFacetWithNormal(anchorPt,farPt,farPtBorder,mesh,true);
                        addFacetWithNormal(farPtBorderPrime,farPtPrime,anchorPtPrime,mesh,false);


//                        for(int k=0;k<points.size()-1;k++){
//                            addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
//                            addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
//                        }
                        //addFacetWithDirection(leftPt,leftBorderPt,leftBorderPtPrime, mesh,direction);
                        //addFacetWithDirection(leftBorderPtPrime, leftPtPrime,leftPt, mesh,direction);

                        //addFacetWithDirection(rightPt,rightBorderPt, rightBorderPtPrime, mesh,direction);
                        //addFacetWithDirection(rightBorderPtPrime, rightPtPrime,rightPt, mesh,direction);




                    }else if(1==numInBounds){
                        FAHVector3 anchorPt, anchorPtPrime;

                        if(b1){
                            direction = p1.copy().sub(p4);
                            anchorPt=p1;
                            anchorPtPrime=p1prime;
                        }else if(b2){
                            direction = p2.copy().sub(p3);
                            anchorPt=p2;
                            anchorPtPrime=p2prime;
                        }else if(b3){
                            direction = p3.copy().sub(p2);
                            anchorPt=p3;
                            anchorPtPrime=p3prime;
                        }else if(b4){
                            direction = p4.copy().sub(p1);
                            anchorPt=p4;
                            anchorPtPrime=p4prime;
                        }

                        for(int k=0;k<points.size()-1;k++){
                            addFacetWithNormal(points[k+1],points[k],anchorPt,mesh,true);
                            addFacetWithNormal(anchorPtPrime,primepoints[k],primepoints[k+1],mesh,false);
                        }




                    }

                    for(int k=0;k<points.size()-1;k++){
                        addFacetWithDirection(points[k],points[k+1],primepoints[k],mesh,direction);
                        addFacetWithDirection(points[k+1],primepoints[k+1],primepoints[k],mesh,direction);
                    }

                    // ADD LOOP POINTS
//                    for(int k=1; k<allpoints.size(); k++){
//                        addFacetWithDirection(allpoints[k-1],allpoints[k],allprimepoints[k-1],mesh, direction);
//                        addFacetWithDirection(allpoints[k],allprimepoints[k-1],allprimepoints[k],mesh,direction);
//                    }


                }else{

                    printf("\n found %i points",points.size());
                    for(int k=0;k<allpoints.size()-1;k++){
                        direction.operator *=(-1);
                        addFacetWithDirection(allpoints[k],allpoints[k+1],allprimepoints[k],mesh,direction);
                        addFacetWithDirection(allpoints[k+1],allprimepoints[k+1],allprimepoints[k],mesh,direction);
                    }

                }
            }
//            if(1<numOnLoop){
//                FAHVector3 direction;
//                qDebug()<<"onloop at "<<i<<","<<j ;
//                if(l1&&l2){//top
//                    direction = p1.copy().sub(p3);
//                    addFacetWithDirection(p1,p2,p2prime,mesh,direction);
//                    addFacetWithDirection(p2prime,p1prime,p1,mesh,direction);

//                }else if(l2&&l4){//right
//                    direction = p2.copy().sub(p1);
//                    addFacetWithDirection(p2,p4,p4prime,mesh,direction);
//                    addFacetWithDirection(p4prime,p2prime,p2,mesh,direction);
//                }else if(l1&&l3){//left
//                    direction = p1.copy().sub(p2);
//                    addFacetWithDirection(p1,p3,p3prime,mesh,direction);
//                    addFacetWithDirection(p3prime,p1prime,p1,mesh,direction);
//                }else if(l3&&l4){//bottom
//                    direction = p3.copy().sub(p1);
//                    addFacetWithDirection(p3,p4,p4prime,mesh,direction);
//                    addFacetWithDirection(p4prime,p3prime,p3,mesh,direction);
//                }
//            }
        }
    }




    // Construct triangles for the borders.

    //addLoopToSTL(OuterLoop,grid,mesh,false);
    //for(int i=0;i<innerLoops.size();i++){
    //    addLoopToSTL(innerLoops.at(i),grid,mesh,true);
    //}
    return mesh;
}

template <class T>
float getHeightAt(float x, float y,XYGrid<T>* grid){
    int i=(int) (x/grid->stepSize());
    int j=(int) (y/grid->stepSize());

    int ip = i+1;
    int jp = j+1;

    if((ip>(grid->nx()-1)) || (jp>(grid->ny()-1))){
        return grid->at(i-1,j-1);
    }

    FAHVector3 p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
    FAHVector3 p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
    FAHVector3 p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
    FAHVector3 p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());

    return getHeightAt(x,y,p1,p2,p3,p4);

}


float getHeightAt(float x, float y, FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, FAHVector3 p4){
    FAHTriangle t1(p1,p2,p4);
    FAHTriangle t2(p1,p3,p4);

    FAHVector3 O(x,y,0);
    FAHVector3 R(0,0,1);
    float h=0;
    if (t1.intersectedByRay(O,R)){
        h=t1.heightAt(O,R);
    }else if(t2.intersectedByRay(O,R)){
        h=t2.heightAt(O,R);
    }else{
        printf("\nCouldnt set value");
        h=p1.z;
    }
    return h;
}

template <class T>
STLMesh* makeSTLfromScan(XYGrid<T>* grid ){
    STLMesh* mesh= new STLMesh();
    for(int j=0;j<grid->ny()-1;j++){
        for(int i=0;i<grid->nx()-1;i++){
            /** calculated vectors of the 4 points and determin case
             *  p1--p2 -->i x  thesebounds
             *  | \  |
             *  p3--p4         nextbounds
             *  |
             * \|/
             *  j
             *  y
             *
             *
             **/
            FAHVector3 p1,p2,p3,p4;
            FAHVector3 p1prime,p2prime,p3prime,p4prime;


            p1=vectorFromIJ(i,j,grid->at(i,j),grid->stepSize());
            p1prime = FAHVector3(&p1);
            p1prime[2]=0;


            p2=vectorFromIJ(i+1,j,grid->at(i+1,j),grid->stepSize());
            p2prime = FAHVector3(&p2);
            p2prime[2]=0;


            p3=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
            p3prime = FAHVector3(&p3);
            p3prime[2]=0;


            p4=vectorFromIJ(i+1,j+1,grid->at(i+1,j+1),grid->stepSize());
            p4prime = FAHVector3(&p4);
            p4prime[2]=0;


            /// Generate top and bottom

            /** MAKE TRIANGLES
             *  p1--p2 --> x
             *  | \ |
             *  p3--p4
             *   |
             *  \|/
             *   Y
             *
             *  x Z
             *  STLs use right hand rule to determin normal
             **/
            /// Make Triangles  p1-p4-p3 and p2-p4-p1 which form the top
            addFacetToSTL(p1,p4,p3,mesh);
            addFacetToSTL(p2,p4,p1,mesh);

            /// Make Triangles p4'-p1'-p3' and p4'-p2'-p1' with Zs=0 to form bottom
            addFacetToSTL(p4prime,p1prime,p3prime,mesh);
            addFacetToSTL(p4prime,p2prime,p1prime,mesh);

            /// BOUNDARIES
            /// NEED CASSES FOR i=0,j=0,i=nx-1,j=ny-1 to make walls
            if ( (0==j) ){ // FRONT section
                /**
                 * p1--p2
                 *  | \ |
                 * p1'--p2'
                 *
                 * need facets p1-p1'-p2' ,   p2-p1-p2'
                **/
                addFacetToSTL(p1,p1prime,p2prime,mesh);
                addFacetToSTL(p2,p1,p2prime,mesh);
            }
            if ( ((grid->ny()-2)==j) ){//BACK
                /**
                 * p3--p4
                 *  | \ |
                 * p3'--p4'
                 *
                 * need facets p3-p3'-p4' ,   p4-p3-p4'
                **/
                addFacetToSTL(p3,p3prime,p4prime,mesh);
                addFacetToSTL(p4,p3,p4prime,mesh);
            }




            if (0==i){//LEFT
                /**
                 * p1--pb
                 *  | \ |
                 * p1'--pb'
                 *
                 * need facets p1-p1'-pb' ,   pb-p1'-pb'
                **/
                FAHVector3 pb, pbprime;
                pb=vectorFromIJ(i,j+1,grid->at(i,j+1),grid->stepSize());
                pbprime = FAHVector3(&pb);
                pbprime[2]=0;

                addFacetToSTL(p1,p1prime,pbprime,mesh);
                addFacetToSTL(pb,p1,pbprime,mesh);
            }
            if (grid->nx()-2==i){//RIGHT
                /**
                 * p2--p4
                 *  | \ |
                 * p2'--p4'
                 *
                 * need facets p1-p1'-pb' ,   pb-p1-pb'
                **/
                addFacetToSTL(p4prime,p4,p2,mesh);
                addFacetToSTL(p2,p2prime,p4prime,mesh);
            }
        }
    }
    return mesh;
}


/**
    // Need to make 5 point average of the border
    QMap<int, QVector<float> > refinedBorder;

    int reach=2;
    for(int j=0; j<grid->ny();j++){

        QVector<int> temp;
        QVector<float> border(2,0);
        QVector<int> tempBorder(2,0);
        QVector<int> totalNum(2,0);
        QVector<float> sum(2,0.0);

        tempBorder = bordermap[j];

        if((j<reach) || (j>(grid->ny()-1-reach)) ){
            border[0]=tempBorder[0]*grid->stepSize();
            border[1]=tempBorder[1]*grid->stepSize();

        }else{
            for(int l=0;l<2;l++){
                for(int k = -reach;k<reach;k++){// Average over last and next number of reach points
                    temp=bordermap[j+k];
                    if( !((temp[l]==0)||(temp[l]==grid->nx())) ){
                        totalNum[l]++;
                        sum[l]=sum[l]+temp[l]*grid->stepSize();
                    }
                }
                if (totalNum[l]>0){
                    border[l]=sum[l]/totalNum[l];
                }else{
                    border[l]=tempBorder[l];
                }
            }
        }
        refinedBorder[j]=border;
    }
    //// New 5 point average for x coordinate of border.
**/



void addFacetToSTL(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh){
    /// need to make sure that no 2 vectors are the same or it will not define a plane

        FAHTriangle t1(p1,p2,p3);
        STLFacet s1;
        s1.triangle=t1;
        s1.normal=t1.normal();
        if(!s1.normal.isInvalid()){
            mesh->AddFacet(s1);
       }

}
FAHLoopInXYPlane smoothLoop(FAHLoopInXYPlane loop, int times, float ylimit){
    FAHLoopInXYPlane newloop(loop);
    int size = loop.points.size();
    float x, y;
//    printf("\nsize:%i",size);
    for(int i=0;i<loop.points.size();i++){
        FAHVector3 pim1;
        if (0==i){
            pim1 = loop.points.last();
        }else{
            pim1 = loop.points.at((i-1)%size);
        }
        FAHVector3 pi = loop.points.at(i%size);
        FAHVector3 pip1 = loop.points.at((i+1)%size);
        FAHVector3 pt;
        x = (pim1.x+pi.x+pip1.x)/3.0;
        y= (pim1.y+pi.y+pip1.y)/3.0;
        if(y>ylimit){
            pt.x = pi.x;
            pt.y = pi.y;
        }else{
            pt.x = x;
            pt.y = y;
        }
        newloop.points[i] = pt;
//        printf("%i ",i);
    }

//    printf("\ntime %i",times);
    times--;

    if(times<=0){return newloop;}
    else{return smoothLoop(newloop,times,ylimit);}

}

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid){
    return boundaryLoopFromGrid(grid,0,50,50);
}

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid, int offsetFromBorder, float slopetrigger, float sensitivity){
    FAHLoopInXYPlane loop;
    QVector<FAHVector3> left, right;
    float max;
    int leftsize=0,rightsize=0;
    bool addedLeft = false, addedRight=false;
//    float sensitivity = 3;


    for(int j=1+offsetFromBorder;j<grid->ny()-1-offsetFromBorder;j++){
        bool leftTrigger = true, rightTrigger = true;
        // find left point
        leftsize = left.size();
        max = grid->at(0,j);
        for(int i=1+offsetFromBorder;i<(grid->nx()-1-offsetFromBorder);i++){
            if (!Math::floatsEqual(max,grid->at(i,j))){
                leftTrigger= true;
//                printf("\nleft triggered");
            }
            if (leftTrigger){
//                printf()
                float backslope = (grid->at(i,j) - grid->at(i-1,j))/grid->stepSize();
                float forslope = (grid->at(i+1,j) - grid->at(i,j))/grid->stepSize();
                float frontslope = (grid->at(i,j+1) - grid->at(i,j))/grid->stepSize();
                float rearslope = (grid->at(i,j) - grid->at(i,j-1))/grid->stepSize();
                float xcurve = (fabs(fabs(forslope)-fabs(backslope)));
                float ycurve = (fabs(fabs(frontslope)-fabs(rearslope)));

                //if( (xcurve<slopetrigger) && fabs(forslope)<sensitivity && fabs(backslope)<sensitivity
                //    && (ycurve<slopetrigger) && fabs(frontslope)<sensitivity && fabs(rearslope)<sensitivity)
                if (grid->at(i,j)>60)
                {
                    left.append(vectorFromIJ(i,j,0,grid->stepSize()));
                    i=grid->nx();
                }
            }
        }
        addedLeft = (left.size()==leftsize);


        // find right point
        rightsize=right.size();
        max = grid->at(grid->nx()-1,j);
        for(int i=(grid->nx()-1-offsetFromBorder); i>1+offsetFromBorder ;i--){
            if (!Math::floatsEqual(max,grid->at(i,j))){
                rightTrigger= true;
            }
            if (rightTrigger){
                float backslope = (grid->at(i,j) - grid->at(i-1,j))/grid->stepSize();
                float forslope = (grid->at(i+1,j) - grid->at(i,j))/grid->stepSize();
                float frontslope = (grid->at(i,j+1) - grid->at(i,j))/grid->stepSize();
                float rearslope = (grid->at(i,j) - grid->at(i,j-1))/grid->stepSize();
                float xcurve = (fabs(fabs(forslope)-fabs(backslope)));
                float ycurve = (fabs(fabs(frontslope)-fabs(rearslope)));

                //if( (xcurve<slopetrigger) && fabs(forslope)<sensitivity && fabs(backslope)<sensitivity
                //    && (ycurve<slopetrigger) && fabs(frontslope)<sensitivity && fabs(rearslope)<sensitivity)
                if (grid->at(i,j)>60)
                {
                    right.append(vectorFromIJ(i,j,0,grid->stepSize()));
                    i=0;
                }
            }
        }
        addedRight = (right.size()==rightsize);

        if (addedRight && ! addedLeft){
            left.append(vectorFromIJ(1+offsetFromBorder,j,0,grid->stepSize()));
        }else if(addedLeft && !addedRight){
            right.append((vectorFromIJ((grid->nx()-2-offsetFromBorder),j,0,grid->stepSize())));
        }

    }
    loop.points = left;
    for(int i=right.size()-1;i>0;i--){
        loop.points.append(right[i]);
    }


//    removedDuplicatePoints(& loop.points);
    return loop;


}

BoundaryMap boundaryGridDetection(XYGrid<float>* grid){
    /// FIND BORDERS
    QMap<int,QVector<int> > bordermap;
    float max;
    bool started= false;
    bool ended = false;
    for(int j=0;j<grid->ny();j++){
        QVector<int> border(2,0);
        bool foundLeft=false;
        bool foundRight=false;
        // FIND left border
        max = grid->at(0,j);
        for(int i=1;i<(grid->nx()-1);i++){
            if (!Math::floatsEqual(max,grid->at(i,j)) && !foundLeft ){
                border[0]=i;
                //printf("\ni:%i\tmax:%f\tval:%f",i,max,grid->at(i,j));
                foundLeft=true;
            }
        }

        //Find right border
        max = grid->at(grid->nx()-1,j);
        for(int i=(grid->nx()-1); i>1 ;i--){
            if ( !Math::floatsEqual(max,grid->at(i,j)) && (!foundRight) ) {
                border[1]=i;
//                printf("\ni:%i\tmax:%f\tval:%f",i,max,grid->at(i,j));
                foundRight=true;
            }
        }

        // if only 1 found, make border edge
        if (foundLeft && !foundRight ){
            border[1] = grid->nx()-1;
            if(!started){started=true;}
        }else if (!foundLeft && foundRight){
            border[0] = 0;
            if(!started){started=true;}
        }
        if(foundLeft&&foundRight&&!started){started=true;}

        if(!foundLeft&&!foundRight&& started){ended=true;}

        if ( foundLeft&&foundRight&& !ended){
            bordermap[j]=border;
//            printf("\n J:%i border: %i,%i",j,border[0],border[1]);
        }
    }
    return bordermap;
}


bool stlToFile(STLMesh* m, QString file){
    STLFile stl;
    stl.SetMesh(m);
    return stl.WriteASCII(file);
}

template STLMesh* makeSTLfromScan(XYGrid<float>* grid );
template STLMesh* makeSTLfromScan(XYGrid<int>* grid );
template STLMesh* makeSTLfromScanSection(XYGrid<float>* grid,const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops);
template STLMesh* makeSTLfromScanSection(XYGrid<int>* grid,const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops);
template void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<float>* grid,STLMesh* mesh, bool inner );
template void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<int>* grid,STLMesh* mesh, bool inner );
template float getHeightAt(float x, float y,XYGrid<float>* grid);
template float getHeightAt(float x, float y,XYGrid<int>* grid);
