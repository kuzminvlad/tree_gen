#ifndef PLANTMAIN_H
#define PLANTMAIN_H

#include "plantrule.h"
#include "plantvertex.h"

class PlantMain
{
public:
    PlantMain(PlantRule pr, string base);
    ~PlantMain(){}

    PlantVertex plantvertex;
    PlantRule plantrule;
    vector<vector<double>> vertexLeaf, vertexLeafPoligon;

    stack<vector<double>> st, stLeaf;
    stack<Tree*> st1;
    stack<map<string, double>> st2;

    Tree *processedTree;

    double angleQ = 90, angleW = 90;
    double d = 100;
    double thicknessT, thicknessB, thicknessS;

    double draw_param = -1, step_count = 1;

    int iter_k = 0;

    int seed;

    void Rx(double angle, double d);
    void Ry(double angle, double d);
    void Rz(double angle, double d);

    void Renew(int n);
    void Move(string base, string nameBase);
    void MoveRecursive(string base, PlantRule plantrule, int n);
    void Update(Tree*&);
    vector<double> Forward(double d, int type = 0); //type = 0 -> tree, type = 1 -> leaf
    double ProcessingParameter(string param, map<string, double> parameters);

    bool point_in_cylinder(vec3 p1, vec3 p2, float r, vec3 q);



};

#endif // PLANTMAIN_H
