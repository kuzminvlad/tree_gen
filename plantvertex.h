#ifndef PLANTVERTEX_H
#define PLANTVERTEX_H

#include <includedlib.h>

struct Tree
{
    vector<double> vertex;
    double trunk_thickness;
    string type_tree;
    vector<Tree*> child;
    Tree *parent;
    map<string, double> parameters;
    int old;
};

class PlantVertex
{
public:
    PlantVertex();
    ~PlantVertex();

    Tree *startTree, *lastTree, *parent;

    Tree* AddTree(Tree *&MyTree, vector<double> ver, double tr_thick, string type, map<string, double> parameters);
    void DelTreeBranch(Tree *&MyTree);
    void DelTreeVertex(Tree *&MyTree);

    bool WriteVertex(string file_path);
    bool ReadVertex(string file_path);
};

#endif // PLANTVERTEX_H
