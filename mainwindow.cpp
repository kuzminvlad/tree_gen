#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QFileDialog>
#include <QTextDocumentWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCurrentFileName(QString(), 1, 0);
    on_pushButton_seed_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::setCurrentFileName(const QString &fileName, int ls, int type)
{
    if (ls == 0)
    {
        if (!QFile::exists(fileName))
            return false;
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly))
            return false;
    }

    if (type == 0)
        fileNameMain = fileName;
    else fileNameLeaf = fileName;

    if (type == 0)
    {
        QString shownName;
        if (fileName.isEmpty())
            shownName = "untitled.rule";
        else
            shownName = QFileInfo(fileName).fileName();

        setWindowTitle(tr("%1[*]").arg(shownName));
        setWindowModified(false);
    }

    return true;
}

void MainWindow::on_action_Open_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Открыть файл..."),QString(), tr("Rule-правило (*.rule);;Все файлы (*)"));
    if (!fn.isEmpty())
    {
        setCurrentFileName(fn, 0, 0);
        mainRule.ReadRule(fileNameMain.toStdString());
    }
    QString strLabel;
    for(int i = fn.size() - 1; i >= 0; i--)
    {
        if (fn[i]=='/') break;
        strLabel = fn[i] + strLabel;
    }
    fnTree = strLabel;
    ui->label_ruleTree->setText("Правило дерева: " + fnTree);
}

void MainWindow::on_action_OpenLeaf_triggered()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Открыть файл..."),QString(), tr("Rule-правило (*.rule);;Все файлы (*)"));
    if (!fn.isEmpty())
    {
        setCurrentFileName(fn, 0, 1);
        leafRule.ReadRule(fileNameLeaf.toStdString());
    }
    QString strLabel;
    for(int i = fn.size() - 1; i >= 0; i--)
    {
        if (fn[i]=='/') break;
        strLabel = fn[i] + strLabel;
    }
    fnLeaf = strLabel;
    ui->label_ruleLeaf->setText("Правило листа: " + fnLeaf);
}

void MainWindow::on_pushButton_draw_clicked()
{
    PlantMain *plant = new PlantMain(mainRule, mainRule.base);
    plant->seed = ui->lineEdit_seed->text().toInt();
    plant->thicknessT = ui->doubleSpinBox_thicknessT->value();
    plant->thicknessB = ui->doubleSpinBox_thicknessB->value();
    plant->thicknessS = ui->doubleSpinBox_thicknessS->value();

    QString fn;
  //  fn.remove(fn.size() - 5, 5);
    for(int i = fileNameMain.size() - 6; i >= 0; i--)
    {
        if (fileNameMain[i] == '/') break;
        fn = fileNameMain[i] + fn;
    }
    fn ="trees//" + fn + "_" + ui->lineEdit_seed->text() + ".vdat";

    plant->Renew(static_cast<int>(mainRule.parameters["g"]));
    plant->plantvertex.WriteVertex(fn.toStdString());

    plant->angleQ  = 90;
    plant->angleW  = 90;
    plant->MoveRecursive("L", leafRule, 10);
    std::ofstream stLeaf;
    stLeaf.open("leaf.vdat");
    for(auto &i : plant->vertexLeafPoligon)
        stLeaf << i[0] << " " << i[1] << " " << i[2] << endl;

    OpenGlDraw *drawScene = new OpenGlDraw(fn, nullptr,false, true, ui->spinBox_quality->value());
    on_pushButton_seed_clicked();
    delete plant;
}

void MainWindow::on_pushButton_draw_2_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Открыть файл..."),QString(), tr("Vertex (*.vdat);;Все файлы (*)"));
    if (!fn.isEmpty())
        OpenGlDraw *drawScene = new OpenGlDraw(fn, nullptr, false, true, ui->spinBox_quality->value());
}

void MainWindow::on_pushButton_generate_clicked()
{
    int k = ui->spinBox->value();
    for(int i = 0; i < k; i++)
    {
        PlantMain *plant = new PlantMain(mainRule, mainRule.base);
        plant->seed = ui->lineEdit_seed->text().toInt();
        plant->thicknessT = ui->doubleSpinBox_thicknessT->value();
        plant->thicknessB = ui->doubleSpinBox_thicknessB->value();
        plant->thicknessS = ui->doubleSpinBox_thicknessS->value();

        QString fn;
        for(int i = fileNameMain.size() - 6; i >= 0; i--)
        {
            if (fileNameMain[i] == '/') break;
            fn = fileNameMain[i] + fn;
        }
        fn = "trees//" + fn +  "_" + ui->lineEdit_seed->text() + ".vdat";


        plant->Renew(static_cast<int>(mainRule.parameters["g"]));
        plant->plantvertex.WriteVertex(fn.toStdString());

        plant->angleQ  = 90;
        plant->angleW  = 90;
        plant->MoveRecursive("L", leafRule, 10);
        std::ofstream stLeaf;
        stLeaf.open("leaf.vdat");
        for(auto &i : plant->vertexLeafPoligon)
            stLeaf << i[0] << " " << i[1] << " " << i[2] << endl;

    if (ui->checkBox->isChecked())
        OpenGlDraw *drawScene = new OpenGlDraw(fn, nullptr, true, false, ui->spinBox_quality->value());
    else
        OpenGlDraw *drawScene = new OpenGlDraw(fn, nullptr, false, false, ui->spinBox_quality->value());

    on_pushButton_seed_clicked();
    delete plant;
    }
}


void MainWindow::on_pushButton_seed_clicked()
{
    srand(time(nullptr));
    int s = rand()%10000000 * rand()%10000000;
    QString str;
    str.setNum(s);
    ui->lineEdit_seed->setText(str);
}

void MainWindow::on_pushButton_mainRule_clicked()
{
    FormRule *formRule = new FormRule(fileNameMain, fnTree, &mainRule);
    formRule->show();
}

void MainWindow::on_pushButton_leafRule_clicked()
{
    FormRule *formRule = new FormRule(fileNameLeaf, fnLeaf, &leafRule);
    formRule->show();
}


void MainWindow::on_action_help_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Справка");
    msgBox.setText("Клавиши управления камерой в виджете отрисовки дерева:\nW,S,A,D - движение вперед, назад, вправо, влево\nзажатая ПКМ с движением  - поворот вверх, вниз, вправо, влево");
    msgBox.exec();
}
