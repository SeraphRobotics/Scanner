#ifndef SCANNERFUNCTIONS_H
#define SCANNERFUNCTIONS_H

#include "stl/stlmesh.h"
#include "xygrid.h"

typedef QMap<int,QVector<int> > BoundaryMap;

/**
 * returns a maping betwen the j(y index) values that have features inside of them,
 * and the boundary values of i for the left and right.
 **/
BoundaryMap boundaryGridDetection(XYGrid<float>* grid);

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid, int offsetFromBorder, float slopetrigger, float sensitivity);

FAHLoopInXYPlane boundaryLoopFromGrid(XYGrid<float>* grid);


FAHLoopInXYPlane smoothLoop(FAHLoopInXYPlane loop, int times, float ylimit);


/// DEGUB FUNCTIONS
void writeLoopToXDFL(const FAHLoopInXYPlane& loop, QString file);
void printPoint(FAHVector3 pt);


/// System for getting height data
float getHeightAt(float x, float y, FAHVector3 p1, FAHVector3 p2, FAHVector3 p3, FAHVector3 p4);

template <class T>
float getHeightAt(float x, float y,XYGrid<T>* grid);

bool loopsContain(FAHVector3 pt, const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops);

/**
 * returns in pointsInGrid all of the points in the grid ij->i+1,j+1 in an ordered list
 **/
bool getPointsInGrid(int i,int j, float stepsize,const FAHLoopInXYPlane& outerloop ,QList<FAHLoopInXYPlane> loops,QVector<FAHVector3>* pointsInGrid);




/// Cleanup helper functions
void removedDuplicatePoints(QVector<FAHVector3>* vector);

void removePointsIfInList(QVector<FAHVector3>* vector, QVector<FAHVector3> *newvector, QList<FAHVector3> points);




/// STL GENERATING FUNCTIONS


/**
 *returns a mesh of the grid.
 *It makes and STL with the heights inside the loop specified
 *heights are specfied by the grid
 **/
template <class T>
STLMesh* makeSTLfromScanSection(XYGrid<T>* grid,const FAHLoopInXYPlane& OuterLoop, QList<FAHLoopInXYPlane> innerLoops);


/**
 *returns a mesh of the grid. It meshes all points in the grid
 **/
template <class T>
STLMesh* makeSTLfromScan(XYGrid<T>* grid );



/// STL Helper functions
template <class T>
void addLoopToSTL(const FAHLoopInXYPlane& loop,XYGrid<T>* grid,STLMesh* mesh,bool inner );

void addFacetWithNormal(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, bool zpositive);

void addFacetWithDirection(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh, FAHVector3 direction);

void addFacetToSTL(FAHVector3 p1,FAHVector3 p2,FAHVector3 p3,STLMesh* mesh);


bool stlToFile(STLMesh* m, QString file);


#endif // SCANNERFUNCTIONS_H
