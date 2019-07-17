#ifndef FORMRULE_H
#define FORMRULE_H

#include <QWidget>
#include <plantrule.h>
#include <includedlib.h>

namespace Ui {
class FormRule;
}

class FormRule : public QWidget
{
    Q_OBJECT

public:
    explicit FormRule(QString file_path, QString ruleName, PlantRule *rule = nullptr, QWidget *parent = nullptr);
    ~FormRule();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::FormRule *ui;
    QString file;
    PlantRule *rule;
};

#endif // FORMRULE_H
