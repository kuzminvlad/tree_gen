#include "plantvertex.h"

PlantVertex::PlantVertex()
{
    startTree = nullptr;
    parent = nullptr;
}

PlantVertex::~PlantVertex()
{
    DelTreeBranch(startTree);
}

Tree *PlantVertex::AddTree(Tree *&MyTree, vector<double> ver, double tr_thick, string type, map<string, double> parameters)
{
    if (MyTree==nullptr)
    {
        MyTree = new Tree;
        MyTree->vertex = ver;
        MyTree->trunk_thickness = tr_thick;
        MyTree->type_tree = type;
        MyTree->parameters = parameters;
        MyTree->parent = parent;
        return MyTree;
    }
    else
    {
        Tree *newTree = nullptr;
        MyTree->child.push_back(newTree);
        parent = MyTree;
        newTree = AddTree(MyTree->child[MyTree->child.size()-1], ver, tr_thick, type, parameters);
        return newTree;
    }
}

void PlantVertex::DelTreeBranch(Tree *&MyTree){
    if (MyTree != nullptr)
    {
        for (ullint i = 0; i < MyTree->child.size(); i++)
           DelTreeBranch(MyTree->child[i]);
        delete MyTree;
    }
}

void PlantVertex::DelTreeVertex(Tree *&MyTree){
    if (MyTree != nullptr)
    {
        for (int i = 0; i < MyTree->parent->child.size(); i++)
        {
            if (MyTree->parent->child[i] == MyTree)
            {
                MyTree->parent->child.erase(MyTree->parent->child.begin() + i);
                break;
            }
        }
        delete MyTree;
    }
}

bool PlantVertex::WriteVertex(string file_path)
{
    fstream fs;
    fs.open(file_path, ios::out);

    stack<Tree*> st;
    stack<int> stnum;

    Tree *tr = startTree;
    for(bool p = true;;)
    {
        if (p)
        {
            for (ullint i = 0; i < 3; i++)
                fs << tr->vertex[i] << " ";
            fs << tr->trunk_thickness << " " << tr->parameters["angleQ"] << " " << tr->parameters["angleW"] << " " << tr->type_tree << endl;
        }

        if ((tr->child.size() == 1)&&(p)) tr = tr->child[0];
        else if ((tr->child.size() > 1)&&(p))
        {
            stnum.push(tr->child.size());
            stnum.push(0);
            st.push(tr);

            fs << "[" << endl;

            tr = tr->child[0];
        }
        else if (((tr->child.size() == 0)&&(!st.empty()))||(!p))
        {
            if (!p) p = true;

            int i = stnum.top();
            stnum.pop();
            int size = stnum.top();
            if (i+1 != size)
            {
                tr = st.top();
                tr = tr->child[i+1];
                stnum.push(i+1);

                fs << "]" << endl  << "[" << endl;
            }
            else
            {
                stnum.pop();
                fs << "]" << endl;
                if (stnum.empty()) break;
                else
                {
                    st.pop();
                    p = false;
                }
            }
        }
        else if ((tr->child.size() == 0)&&(st.empty())) break;
    }
    return true;
}

bool PlantVertex::ReadVertex(string file_path)
{
    fstream fs;
    fs.open(file_path, ios::in);
    Tree *tr = nullptr;
    startTree = nullptr;

    stack<Tree*> st;

    vector<double> v;
    while(fs)
    {
        string str;
        getline(fs,str);
        if (str[0] == '[')
        {
            st.push(tr);
        }
        else if (str[0] == ']')
        {
            tr = st.top();
            st.pop();
        }
        else if (str != "")
        {
            stringstream ost;
            ost << str;
            double thick, angleQ, angleW;
            string type;
            v.resize(3);
            ost >> v[0] >> v[1] >> v[2] >> thick >> angleQ >> angleW >> type;

            map<string, double> parameters;
            parameters["angleQ"] = angleQ;
            parameters["angleW"] = angleW;

            tr = AddTree(tr, v, thick, type, parameters);
            if (startTree == nullptr) startTree = tr;
        }
    }
    return true;
}
