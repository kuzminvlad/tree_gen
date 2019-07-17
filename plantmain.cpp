#include "plantmain.h"

PlantMain::PlantMain(PlantRule pr, string base) : plantrule(pr)
{
    vector<double> a = { 0, 0, 0};
    plantrule.parameters["angleQ"] = angleQ;
    plantrule.parameters["angleW"] = angleW;
    plantrule.parameters["RX"] = 0;
    plantrule.parameters["RY"] = 0;
    plantrule.parameters["RZ"] = 0;
    plantvertex.startTree = plantvertex.AddTree(plantvertex.startTree, a, 1, "t", plantrule.parameters);
    plantvertex.startTree->old = 0;
    plantvertex.lastTree = plantvertex.AddTree(plantvertex.startTree, a, 1, "t" + base + "!", plantrule.parameters);
    plantvertex.lastTree->old = 0;

    vector<double> l = {0, 0, 0};
    vertexLeaf.push_back(l);
}

void PlantMain::Rz(double angle, double d)
{
    double t_cos = cos(angle * PI / 180), t_sin = sin(angle * PI / 180);
    vector<double> oldCoord = Forward(d);
    vector<vector<double>> rx = { { 1, 0, 0 } ,{ 0, t_cos, t_sin },{ 0, -t_sin, t_cos } };

    for (ullint i = 0; i < 3; i++)
        oldCoord[i] -= plantvertex.lastTree->vertex[i];

    vector<double> newCoord = {plantvertex.lastTree->vertex[0], plantvertex.lastTree->vertex[1], plantvertex.lastTree->vertex[2] };
    for (ullint i = 0; i < 3; i++)
    {
        for (ullint j = 0; j < 3; j++)
            newCoord[i] += rx[i][j] * oldCoord[j];
        newCoord[i] -= plantvertex.lastTree->vertex[i];
    }
    if ((newCoord[0] > 0) && (newCoord[1] >= 0)) angleQ = atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if (newCoord[0] <= 0)  angleQ = 180 + atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if ((newCoord[0] >= 0) && (newCoord[1] < 0)) angleQ = 360 + atan(newCoord[1] / newCoord[0]) * 180 / PI;

    angleW = acos(newCoord[2] / sqrt(pow(newCoord[0], 2) + pow(newCoord[1], 2) + pow(newCoord[2], 2))) * 180 / PI;
}

void PlantMain::Ry(double angle, double d)
{
    double t_cos = cos(angle * PI / 180), t_sin = sin(angle * PI / 180);
    vector<double> oldCoord = Forward(d);
    vector<vector<double>> ry = { { t_cos, 0, -t_sin },{ 0, 1, 0 },{ t_sin, 0, t_cos } };

    for (ullint i = 0; i < 3; i++)
        oldCoord[i] -= plantvertex.lastTree->vertex[i];

    vector<double> newCoord = {plantvertex.lastTree->vertex[0], plantvertex.lastTree->vertex[1],plantvertex.lastTree->vertex[2] };
    for (ullint i = 0; i < 3; i++)
    {
        for (ullint j = 0; j < 3; j++)
            newCoord[i] += ry[i][j] * oldCoord[j];
        newCoord[i] -= plantvertex.lastTree->vertex[i];
    }

    if ((newCoord[0] > 0) && (newCoord[1] >= 0)) angleQ = atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if (newCoord[0] <= 0)  angleQ = 180 + atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if ((newCoord[0] >= 0) && (newCoord[1] < 0)) angleQ = 360 + atan(newCoord[1] / newCoord[0]) * 180 / PI;

    angleW = acos(newCoord[2] / sqrt(pow(newCoord[0], 2) + pow(newCoord[1], 2) + pow(newCoord[2], 2))) * 180 / PI;

}

void PlantMain::Rx(double angle, double d)
{
    double t_cos = cos(angle * PI / 180), t_sin = sin(angle * PI / 180);
    vector<double> oldCoord = Forward(d);
    vector<vector<double>> rz = { { t_cos, t_sin, 0 } ,{ -t_sin, t_cos, 0 } ,{ 0, 0, 1 } };

    for (ullint i = 0; i < 3; i++)
        oldCoord[i] -= plantvertex.lastTree->vertex[i];

    vector<double> newCoord = {plantvertex.lastTree->vertex[0], plantvertex.lastTree->vertex[1], plantvertex.lastTree->vertex[2] };
    for (ullint i = 0; i < 3; i++)
    {
        for (ullint j = 0; j < 3; j++)
            newCoord[i] += rz[i][j] * oldCoord[j];
        newCoord[i] -= plantvertex.lastTree->vertex[i];
    }

    if ((newCoord[0] > 0) && (newCoord[1] >= 0)) angleQ = atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if (newCoord[0] < 0)  angleQ = 180 + atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if ((newCoord[0] > 0) && (newCoord[1] < 0)) angleQ = 360 + atan(newCoord[1] / newCoord[0]) * 180 / PI;
    else if ((fabs(newCoord[0]) < 0.0001) && (fabs(newCoord[1]) < 0.0001))
    {
        angleQ += angle;
        if (angleQ >= 360) angleQ -= 360;
        if (angleQ < 0) angleQ += 360;
    }

    angleW = acos(newCoord[2] / sqrt(pow(newCoord[0], 2) + pow(newCoord[1], 2) + pow(newCoord[2], 2))) * 180 / PI;
}

void PlantMain::Renew(int n)
{
    srand(seed);
    iter_k = 0;
    for (int i = 0; i < n; i++, iter_k++)
    {
        std::queue<Tree*> q1,q2;
        q1.push(plantvertex.startTree);
        while(!q1.empty())
        {
            while(!q1.empty())
            {
                Tree *tr = q1.front();
                q1.pop();

                for (ullint j = 0; j < tr->child.size(); j++)
                {
                    q2.push(tr->child[j]);
                }
                if ((tr->type_tree[tr->type_tree.size() - 1] == '!') || (tr->type_tree[tr->type_tree.size() - 1] == 'F'))
                {
                    plantvertex.lastTree = tr->parent;

                    string base, nameBase;
                    if (tr->type_tree[tr->type_tree.size() - 1] == '!')
                    {
                        string b(tr->type_tree.begin() + 1,tr->type_tree.end()-1);
                        base = b;
                        nameBase = tr->type_tree[0];
                    }
                    else
                    {
                        string b(tr->type_tree.begin() + 1,tr->type_tree.end()-2);
                        base = b;
                        nameBase = tr->type_tree[0];
                    }

                    processedTree = tr;
                    angleQ = plantvertex.lastTree->parameters["angleQ"];
                    angleW = plantvertex.lastTree->parameters["angleW"];

                    Move(base, nameBase);
                   // Update(trParent);
                    Update(plantvertex.startTree);
                }
            }
            q1 = q2;
            while (!q2.empty()) q2.pop();
        }
    }
}

void PlantMain::Update(Tree*& treeStart)
{
    std::queue<Tree*> q1,q2;
    q1.push(treeStart);
    bool trStartP = true;
    while(!q1.empty())
    {
        while(!q1.empty())
        {
            Tree *tr = q1.front();
            q1.pop();

            for (ullint j = 0; j < tr->child.size(); j++)
            {
                plantvertex.lastTree = tr;
                angleQ = tr->parameters["angleQ"];
                angleW = tr->parameters["angleW"];

                if(fabs(tr->child[j]->parameters["RX"]) > 0.0001) Rx(tr->child[j]->parameters["RX"], tr->parameters["l"]);
                if(fabs(tr->child[j]->parameters["RY"]) > 0.0001) Ry(tr->child[j]->parameters["RY"], tr->parameters["l"]);
                if(fabs(tr->child[j]->parameters["RZ"]) > 0.0001) Rz(tr->child[j]->parameters["RZ"], tr->parameters["l"]);
                tr->child[j]->parameters["angleQ"] = angleQ;
                tr->child[j]->parameters["angleW"] = angleW;
                if (tr->child[j]->type_tree[tr->child[j]->type_tree.size() - 1] == '!')
                    tr->child[j]->vertex = tr->vertex;
                else
                {
                    vector<double> a = Forward(tr->parameters["l"]);
                    tr->child[j]->vertex = a;

                    int old = iter_k - tr->child[j]->old;
                    int allYear = tr->parameters["g"];
                    if(trStartP)
                    {
                        tr->trunk_thickness = thicknessT + 15 * thicknessT * (iter_k - tr->old) / allYear;
                        trStartP = false;
                    }
                    if(tr->child[j]->type_tree[0] == 't')
                    {
                        tr->child[j]->trunk_thickness = thicknessT + 15 * thicknessT * old / allYear;
                    }
                    else if(tr->child[j]->type_tree[0] == 'b')
                    {
                        tr->child[j]->trunk_thickness = thicknessB + 5 * thicknessB * old / allYear;
                    }
                    else if(tr->child[j]->type_tree[0] == 's')
                    {
                        tr->child[j]->trunk_thickness = thicknessS + thicknessS * old / allYear;
                    }
                    else if(tr->child[j]->type_tree[0] == 'l')
                    {
                        tr->child[j]->trunk_thickness = 0.1;
                    }
                    else
                    {
                        tr->child[j]->trunk_thickness = thicknessB + 5 * thicknessB * old / allYear * 0.4;
                    }

                    bool cp = false;
                    for(ullint jj = 0; jj < tr->child[j]->child.size(); jj++)
                    {
                        string str = tr->child[j]->child[jj]->type_tree;
                        if (str == "t" || str == "b" || str == "s" || str == "l") cp = true;
                    }
                    if (cp == false) tr->child[j]->trunk_thickness = 0.1;
                }

                q2.push(tr->child[j]);
            }
        }
        q1 = q2;
        while (!q2.empty()) q2.pop();
    }
}

void PlantMain::Move(string base, string nameBase)
{
    //plantrule.rule["Имя_правила"].mas_rules[№_F0,F1,...]["Имя_куска_правила"]=кусок_правила
    //predecessor<<NRule>>descendant:condition->rule:probability
    //plantrule.parametrs["Имя"]=значение(double)

    map<string, double> parameters = plantvertex.lastTree->parameters;
    parameters["k"] = iter_k;

    bool p_processedTree = true;
    double rX = 0, rY = 0, rZ = 0;
    for (ullint i = 0; i < base.size(); i++)
    {
        if (base[i] == '{')
        {
            int k = 1;
            string ps;
            for (string str = "";; i++)
            {
                if (base[i] == '{') continue;
                if (k == 1)
                {
                    ps += base[i];
                    k++;
                    continue;
                }
                if (k == 2)
                {
                    k = 0;
                    continue;
                }
                if (base[i] == ',')
                {
                    double rez = ProcessingParameter(str, parameters);
                    parameters[ps] = rez;
                    plantvertex.lastTree->parameters[ps] = rez;
                    ps = "";
                    str = "";
                    k = 1;
                    continue;
                }
                if (base[i] == '}')
                {
                    double rez = ProcessingParameter(str, parameters);
                    parameters[ps] = rez;
                    plantvertex.lastTree->parameters[ps] = rez;
                    break;
                }
                str += base[i];
            }
        }
        bool pF = true;
        for (auto &r : plantrule.rule)
        {
            //сделать проверку на predecessor, descendant, condition
            if (base[i] == r.first[0])
            {
                if (base[i] == 'F') pF = false;

                vector<map<string,string>> setRule;
                double sumProbability = 0;
                for(auto &sr : r.second.mas_rules)
                {
                    if (sr["predecessor"] != "" && i == 0) continue;
                    else if (sr["predecessor"] != "" && i != 0)
                    {
                        string s;
                        s += base[i-1];
                        if (sr["predecessor"] != s) continue;
                    }
                    if (sr["descendant"] != "" && i == (base.size() - 1)) continue;
                    else if (sr["descendant"] != "" && i != (base.size() - 1))
                    {
                        string s;
                        s += base[i+1];
                        if (sr["descendant"] != s) continue;
                    }
                    if ((sr["condition"] != "") && (fabs(ProcessingParameter(sr["condition"], parameters))) < 0.001) continue;

                    setRule.push_back(sr);
                    double tp = ProcessingParameter(sr["probability"], parameters);
                    sumProbability += tp;
                }
                sumProbability = 100 / sumProbability;

                vector<double> mapProbability;
                double smp = 0;
                for (auto &mp : setRule)
                {
                    double tp = ProcessingParameter(mp["probability"], parameters);
                    smp += tp * sumProbability;
                    mapProbability.push_back(smp);
                }
                int p = rand() % 101;
                int k = (mapProbability.size())? 0 : -1;
                if (mapProbability.size() > 1)
                for (int mp = 0; mp < mapProbability.size() - 1; mp++)
                {
                    if ((p >= mapProbability[mp]) && (p < mapProbability[mp + 1]))
                    {
                        k = mp + 1;
                        break;
                    }
                }

                if (k != -1)
                {
                    if(base[i] == 'T')
                        plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, plantvertex.lastTree->vertex, 1, "t" + setRule[k]["rule"] + "!", parameters);
                    else if(base[i] == 'B')
                        plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, plantvertex.lastTree->vertex, 1, "b" + setRule[k]["rule"] + "!", parameters);
                    else if(base[i] == 'S')
                        plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, plantvertex.lastTree->vertex, 1, "s" + setRule[k]["rule"] + "!", parameters);
                    else if (base[i] != 'F')
                        plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, plantvertex.lastTree->vertex, 1, "z" + setRule[k]["rule"] + "!", parameters);

                    plantvertex.lastTree->parameters["angleQ"] = angleQ;
                    plantvertex.lastTree->parameters["angleW"] = angleW;
                    plantvertex.lastTree->parameters["RX"] = rX;
                    plantvertex.lastTree->parameters["RY"] = rY;
                    plantvertex.lastTree->parameters["RZ"] = rZ;
                    plantvertex.lastTree->old = iter_k;
                    rX = rY = rZ = 0;

                    if(p_processedTree)
                    {
                        plantvertex.lastTree->parameters["RX"] += processedTree->parameters["RX"];
                        plantvertex.lastTree->parameters["RY"] += processedTree->parameters["RY"];
                        plantvertex.lastTree->parameters["RZ"] += processedTree->parameters["RZ"];
                        p_processedTree = false;
                    }

                    if (base[i] == 'F') plantvertex.lastTree->type_tree += "F";
                }
            }
        }
        if ((base[i] == 'F' && pF)||(base[i] == 'L'))
        {
            vector<double> a = Forward(parameters["l"]);
            if (base[i] != 'L') plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, a, 1, nameBase, parameters);
            else
            {
                plantvertex.lastTree = plantvertex.AddTree(plantvertex.lastTree, a, 1, "l", parameters);
            }
            plantvertex.lastTree->parameters["angleQ"] = angleQ;
            plantvertex.lastTree->parameters["angleW"] = angleW;
            plantvertex.lastTree->parameters["RX"] = rX;
            plantvertex.lastTree->parameters["RY"] = rY;
            plantvertex.lastTree->parameters["RZ"] = rZ;
            plantvertex.lastTree->old = iter_k;
            rX = rY = rZ = 0;

            if(p_processedTree)
            {
                plantvertex.lastTree->parameters["RX"] += processedTree->parameters["RX"];
                plantvertex.lastTree->parameters["RY"] += processedTree->parameters["RY"];
                plantvertex.lastTree->parameters["RZ"] += processedTree->parameters["RZ"];
                p_processedTree = false;
            }
        }
        if (base[i] == '+') rX += plantvertex.lastTree->parameters["a"];
        if (base[i] == '-') rX -= plantvertex.lastTree->parameters["a"];
        if (base[i] == '&') rY += plantvertex.lastTree->parameters["a"];
        if (base[i] == '^') rY -= plantvertex.lastTree->parameters["a"];
        if (base[i] == '#') rZ += plantvertex.lastTree->parameters["a"];
        if (base[i] == '$') rZ -= plantvertex.lastTree->parameters["a"];
        if (base[i] == '|') rX += 180;

        if (base[i] == '[')
        {
            vector<double> a = {angleQ, angleW, rX, rY, rZ};
            st.push(a);
            st1.push(plantvertex.lastTree);
            st2.push(parameters);
        }
        if (base[i] == ']')
        {
            vector<double> a = st.top();
            st.pop();
            angleQ = a[0];
            angleW = a[1];
            rX = a[2];
            rY = a[3];
            rZ = a[4];

            plantvertex.lastTree = st1.top();
            parameters  = st2.top();
            st1.pop();
            st2.pop();
        }
    }

    for(ullint k = 0; k < processedTree->child.size(); k++)
        plantvertex.lastTree->child.push_back(processedTree->child[k]);
    for(ullint k = 0; k < plantvertex.lastTree->child.size(); k++)
        plantvertex.lastTree->child[k]->parent = plantvertex.lastTree;
    plantvertex.DelTreeVertex(processedTree);
}

void PlantMain::MoveRecursive(string base, PlantRule plantrule, int n)
{
    //plant.rule["Имя_правила"].mas_rules[№_F0,F1,...]["Имя_куска_правила"]=кусок_правила
    //predecessor<<NRule>>descendant:condition->rule:probability
    //plant.parametrs["Имя"]=значение(double)
    for (ullint i = 0; i < base.size(); i++)
    {
        if (base[i] == '{')
        {
            int k = 1;
            string ps;
            for (string str = "";; i++)
            {
                if (base[i] == '{') continue;
                if (k == 1)
                {
                    ps += base[i];
                    k++;
                    continue;
                }
                if (k == 2)
                {
                    k = 0;
                    continue;
                }
                if (base[i] == ',')
                {
                    double rez = ProcessingParameter(str,plantrule.parameters);
                    plantrule.parameters[ps] = rez;
                    ps = "";
                    str = "";
                    k = 1;
                    continue;
                }
                if (base[i] == '}')
                {
                    double rez = ProcessingParameter(str, plantrule.parameters);
                    plantrule.parameters[ps] = rez;
                    break;
                }
                str += base[i];
            }
        }
        bool pF = true;
        for (auto &r : plantrule.rule)
        {
            //сделать проверку на predecessor, descendant, condition
           // int p = seed % r.second.mas_rules.size();
            if ((base[i] == r.first[0]) && n)
            {
                if (base[i] == 'F') pF = false;

                vector<map<string,string>> setRule;
                double sumProbability = 0;
                for(auto &sr : r.second.mas_rules)
                {
                    if (sr["predecessor"] != "" && i == 0) continue;
                    else if (sr["predecessor"] != "" && i != 0)
                    {
                        string s;
                        s += base[i-1];
                        if (sr["predecessor"] != s) continue;
                    }
                    if (sr["descendant"] != "" && i == (base.size() - 1)) continue;
                    else if (sr["descendant"] != "" && i != (base.size() - 1))
                    {
                        string s;
                        s += base[i+1];
                        if (sr["descendant"] != s) continue;
                    }
                    if ((sr["condition"] != "") && (!ProcessingParameter(sr["condition"], plantrule.parameters))) continue;

                    setRule.push_back(sr);
                    string s = sr["probability"];
                    stringstream ost;
                    ost << s;
                    double tp;
                    ost >> tp;
                    sumProbability += tp;
                }
                sumProbability = 100 / sumProbability;

                vector<double> mapProbability;
                double smp = 0;
                for (auto &mp : setRule)
                {
                    string s = mp["probability"];
                    stringstream ost;
                    ost << s;
                    double tp;
                    ost >> tp;
                    smp += tp * sumProbability;
                    mapProbability.push_back(smp);
                }
                int p = rand() % 101;
                int k = (mapProbability.size())? 0 : -1;
                if (mapProbability.size() > 1)
                for (int mp = 0; mp < mapProbability.size() - 1; mp++)
                {
                    if ((p >= mapProbability[mp]) && (p < mapProbability[mp + 1]))
                    {
                        k = mp + 1;
                        break;
                    }
                }
                if (k != -1 ) MoveRecursive(setRule[k]["rule"], plantrule, n - 1);
            }
        }
        if (base[i] == 'F' && pF)
        {
            vector<double> a = Forward(plantrule.parameters["l"], 1);
            a.push_back(++step_count);
            vertexLeaf.push_back(a);
        }
        if (base[i] == '+') Rx( plantrule.parameters["a"], plantrule.parameters["l"]); //Turn lef
        if (base[i] == '-') Rx(-plantrule.parameters["a"], plantrule.parameters["l"]); //Turn righ
        if (base[i] == '&') Ry( plantrule.parameters["a"], plantrule.parameters["l"]); //Pitch down
        if (base[i] == '^') Ry(-plantrule.parameters["a"], plantrule.parameters["l"]); //Pitch up
        if (base[i] == '#') Rz( plantrule.parameters["a"], plantrule.parameters["l"]); //Roll left
        if (base[i] == '$') Rz(-plantrule.parameters["a"], plantrule.parameters["l"]); //Roll right
        if (base[i] == '|') Rx(180, plantrule.parameters["l"]); //Turn around
        if (base[i] == '[')
        {
            vector<double> a = {vertexLeaf[vertexLeaf.size() - 1][0], vertexLeaf[vertexLeaf.size() - 1][1],
                                vertexLeaf[vertexLeaf.size() - 1][2], angleQ, angleW, step_count, static_cast<double>(vertexLeaf.size() - 1)};
            st.push(a);
        }
        if (base[i] == ']')
        {
            vector<double> a = st.top();
            st.pop();
            vector<double> b = {a[0], a[1], a[2], a[6], a[5]};
            vertexLeaf.push_back(b);
            angleQ = a[3];
            angleW = a[4];
            step_count = a[5];
        }
        if (base[i] == '.')
        {
            vector<double> a = {vertexLeaf[vertexLeaf.size() - 1][0], vertexLeaf[vertexLeaf.size() - 1][1], vertexLeaf[vertexLeaf.size() - 1][2]};
            stLeaf.push(a);
        }
        if (base[i] == ')')
        {
            if (stLeaf.size() == 3)
                while(stLeaf.size() > 0)
                {
                    vertexLeafPoligon.push_back(stLeaf.top());
                    stLeaf.pop();
                }
            else if (stLeaf.size() > 3)
            {
                vector<double> vertexFirst = stLeaf.top();
                stLeaf.pop();
                vector<double> vertexSecond = stLeaf.top();
                stLeaf.pop();
                while (stLeaf.size() > 0)
                {
                   vertexLeafPoligon.push_back(vertexFirst);
                   vertexLeafPoligon.push_back(vertexSecond);
                   vertexLeafPoligon.push_back(stLeaf.top());
                   vertexSecond = stLeaf.top();
                   stLeaf.pop();
                }
            }
        }
    }
}

vector<double> PlantMain::Forward(double d, int type)
{
    vector<double> a;
    if (type == 0)
        a = plantvertex.lastTree->vertex;
    else if (type == 1)
        a = {vertexLeaf[vertexLeaf.size() - 1][0], vertexLeaf[vertexLeaf.size() - 1][1], vertexLeaf[vertexLeaf.size() - 1][2]};
    double x = a[0], y = a[1], z = a[2];
    double cosQ = cos(angleQ * PI / 180), sinQ = sin(angleQ * PI / 180), cosW = cos(angleW * PI / 180), sinW = sin(angleW * PI / 180);
    x += d * sinW * cosQ;
    y += d * sinW * sinQ;
    z += d * cosW;

    vector<double> b = { x, y, z };
    return b;
}

bool PlantMain::point_in_cylinder(vec3 p1, vec3 p2, float r, vec3 q)
{
    //(q-p1)*(p2-p1) >= 0
    //(q-p2)*(p1-p2) >= 0
    float t1 = glm::dot(q-p1, p2-p1);
    float t2 = glm::dot(q-p2, p1-p2);

    //|(q-p1)x(p2-p1)| / |p2-p1| <= r
    vec3 cr = glm::cross(q-p1, p2-p1);
    double t3 = glm::length(cr) / glm::length(p2-p1);

    return  (t1 >= 0) && (t2 >= 0) && (t3 <= r);
}

double PlantMain::ProcessingParameter(string param, map<string, double> parameters)
{
    //обрабатывает +,-,*,/,^,<,>,<=,>=,!=,==,&&,||   приоритет только между +,-и *,/,^ иначе пользоваться скобками

    stack<char> st;
    string s;
    int numberK = 0;

    for(ullint i = 0; i < param.size(); i++)
    {
        if (isdigit(param[i]))
        {
            while(isdigit(param[i]) || param[i] == '.')
            {
                s += param[i];
                i++;
                if (i == param.size()) break;
            }
            s += " ";
            numberK++;
            i--;
        }
        else if (isalpha(param[i]))
        {
            string str;
            str += param[i];
            double n = parameters[str];
            stringstream ost;
            ost << n;
            s += ost.str() + " ";
            numberK++;
        }
        else if (param[i] == '(')
            st.push(param[i]);
        else if (param[i] == ')')
        {
            char sp = st.top();
            st.pop();
            while (sp != '(')
            {
                s += sp;
                s += " ";
                sp = st.top();
                st.pop();
            }
        }
        else
        {
            if (st.size() > 0)
                if ((param[i] == '-' || param[i] == '+') && (st.top() == '*' || st.top() == '/' || st.top() == '^'))
                {
                    s += st.top();
                    s += " ";
                    st.pop();
                }
                else if (((param[i] == '&')&&(param[i+1] == '&'))||((param[i] == '|')&&(param[i+1] == '|')))
                {
                    st.push(param[i]);
                    i++;
                }
                else st.push(param[i]);
            else
            {
                if (((param[i] == '&')&&(param[i+1] == '&'))||((param[i] == '|')&&(param[i+1] == '|')))
                {
                    st.push(param[i]);
                    i++;
                }
                else
                    st.push(param[i]);
            }
        }
    }

    if (numberK == 1)
    {
        stringstream ost;
        ost << s;
        double n;
        ost >> n;
        return n;
    }


    while (st.size() > 0)
    {
        s += st.top();
        s += " ";
        st.pop();
    }

    double result = 0;
    stack<double> temp;
    bool equal = false;
    double a = 0.0, b = 0.0;
    for(ullint i = 0; i < s.size(); i++)
    {
        if (isdigit(s[i]) || s[i] == '.')
        {
            string str;
            while(isdigit(s[i]) || s[i] == '.')
            {
                str += s[i];
                i++;
                if (i == s.size()) break;
            }
            stringstream ost;
            ost << str;
            double n;
            ost >> n;
            temp.push(n);
            i--;
        }
        else if (s[i] != ' ')
        {
            if (!equal)
            {
                a = temp.top();
                temp.pop();
                b = temp.top();
                temp.pop();
            }
            switch (s[i])
            {
            case '+': result = b + a;
                break;
            case '-': result = b - a;
                break;
            case '*': result = b * a;
                break;
            case '/': result = b / a;
                break;
            case '^': result = pow(b,a);
                break;
            case '>':
            {
                if (equal)
                {
                    result = b >= a;
                    equal = false;
                }
                else result = b > a;
                break;
            }
            case '<':
            {
                if (equal)
                {
                    result = b <= a;
                    equal = false;
                }
                else result = b < a;
                break;
            }
            case '!':
            {
                if (equal)
                {
                    result = (b - a) > 0.0001;
                    equal = false;
                }
                break;
            }
            case '=':
            {
                if (equal)
                {
                    result = (b - a) < 0.0001;
                    equal = false;
                }
                else equal = true;
                break;
            }
            case '&':
            {
                if ((abs(a) > 0.001) && (abs(b) > 0.001))
                    result = 1;
                else result = 0;
            }
                break;
            case '|':
            {
                if ((abs(a) > 0.001) || (abs(b) > 0.001))
                    result = 1;
                else result = 0;
            }
                break;
            }
            if (!equal) temp.push(result);
        }
    }
    return result;
}
