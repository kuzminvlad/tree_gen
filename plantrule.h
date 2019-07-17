#ifndef PLANTRULE_H
#define PLANTRULE_H

#include <includedlib.h>

struct rules
{
    vector<map<string,string>> mas_rules;
    /*
    map<string,string> mp;
    mp["rule"]= само правило
    mp["predecessor"]=  предок
    mp["descendant"]=  потомок
    mp["condition"]=  условие
    mp["probability"]= коэффициент вероятности при наличие нескольких удовлетворяющих

    предок<<имя_правила(x1,..)>>потомок:условие->правило:вероятность

    */

};

class PlantRule
{
public:
    PlantRule();
    ~PlantRule();

    string name, base;
    map<string, rules> rule;         //rule["Имя_правила"].mas_rules[№_F0,F1,...]["Имя_куска_правила"]=кусок_правила
    map<string, double> parameters;

    bool ReadRule(string file_path);
    bool WriteRule(string file_path);

    bool treeRule;
};


#endif // PLANTRULE_H
