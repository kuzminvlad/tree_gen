#include "plantrule.h"

PlantRule::PlantRule()
{
}

PlantRule::~PlantRule()
{
}

bool PlantRule::ReadRule(string file_path)
{
    rule.clear();
    parameters.clear();

    fstream fs;
    fs.open(file_path, ios::in);

    while (fs)
    {
        char c;
        fs >> c;
        if (c == '#') break;
    }
    string str;
    fs >> str;
    if (str == "tree")
    {
        treeRule = true;
        fs >> str;
    }
    else treeRule = false;
    //#name
    fs >> name;
    fs >> str; //#base
    fs >> base;
    fs >> str; //#rules

    while(1)
    {
        fs >> str;
        if (str == "#parameters") break;
        if (str == "#end")
        {
            fs.close();
            return true;
        }
        string s, s1;
        int p = 0;
        //rule["Имя_правила"].mas_rules[№_F0,F1,...]["Имя_куска_правила"]=кусок_правила
        //predecessor<<NRule>>descendant:condition->rule:probability
        for(ullint i = 0; i < str.size(); i++)
        {
            if (str[i] == '<' && str[i+1] == '<')
            {
                s1 = s;
                i++;
                s = "";
                continue;
            }
            if (str[i] == '>' && str[i+1] == '>')
            {
                map<string,string> mp;
                mp["predecessor"] = s1;
                rule[s].mas_rules.push_back(mp);
                s1 = s;
                s = "";
                i++;
                continue;
            }
            if (str[i] == ':' && p == 0)
            {
                rule[s1].mas_rules[rule[s1].mas_rules.size()-1]["descendant"] = s;
                s = "";
                p++;
                continue;
            }
            if (str[i] == '-' && str[i+1] == '>')
            {
                rule[s1].mas_rules[rule[s1].mas_rules.size()-1]["condition"] = s;
                s = "";
                i++;
                continue;
            }
            if (str[i] == ':' && p == 1)
            {
                rule[s1].mas_rules[rule[s1].mas_rules.size()-1]["rule"] = s;
                s = "";
                continue;
            }
            if (i == str.size()-1)
            {
                s+=str[i];
                rule[s1].mas_rules[rule[s1].mas_rules.size()-1]["probability"] = s;
            }
            s+=str[i];
        }
    }
    while(1)
    {
        fs >> str;
        if (str == "#end")
        {
            fs.close();
            return true;
        }
        double d;
        fs >> d;
        parameters[str] = d;
    }
}

bool PlantRule::WriteRule(string file_path)
{
    fstream fs;
    fs.open(file_path, ios::out);

    fs << "#name" << endl;
    fs << name << endl;
    fs << "#base" << endl;
    fs << base << endl;
    fs << "#rules" << endl;

    for (auto &i : rule)
        for (auto &j : i.second.mas_rules)
        {
            fs << j["predecessor"];
            fs << "<<";
            fs << i.first;
            fs << ">>";
            fs << j["descendant"];
            fs << ":";
            fs << j["condition"];
            fs << "->";
            fs << j["rule"];
            fs << ":";
            fs << j["probability"];
            fs << endl;
        }
    fs << "#parameters" << endl;
    for (auto &i : parameters)
    {
        fs << i.first << " ";
        fs << i.second << endl;
    }
    fs << "#end";

    fs.close();
    return true;
}
