#include "formrule.h"
#include "ui_formrule.h"

FormRule::FormRule(QString file_path, QString ruleName, PlantRule *rule, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormRule)
{
    ui->setupUi(this);
    setWindowTitle(ruleName);
    this->rule = rule;

    file = file_path;

    std::ifstream fs;
    fs.open(file.toStdString());

    string str;
    while(fs)
    {
        char c = fs.get();
        str = str + c;
    }

    QString qstr;
    qstr = qstr.fromStdString(str);
    qstr.remove(qstr.size() - 1, 1);
    ui->textEdit->textCursor().insertText(qstr);
    fs.close();

}

FormRule::~FormRule()
{
    delete ui;
}

void FormRule::on_pushButton_save_clicked()
{
    QString qstr = ui->textEdit->document()->toPlainText();
    string str = qstr.toStdString();
    std::ofstream fs;
    fs.open(file.toStdString());
    fs << str;
    fs.close();
    rule->ReadRule(file.toStdString());
    this->close();
}

void FormRule::on_pushButton_close_clicked()
{
    this->close();
}
