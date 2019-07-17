#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMessageBox>

#include <opengldraw.h>
#include <formrule.h>


#include "plantmain.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString fileNameMain, fileNameLeaf, fnTree, fnLeaf;
    bool treeRule;

private slots:
    void on_action_Open_triggered();

    void on_action_OpenLeaf_triggered();

    void on_pushButton_draw_clicked();

    void on_pushButton_draw_2_clicked();

    void on_pushButton_seed_clicked();

    void on_pushButton_mainRule_clicked();

    void on_pushButton_leafRule_clicked();

    void on_pushButton_generate_clicked();

    void on_action_help_triggered();

private:
    Ui::MainWindow *ui;
    PlantRule mainRule, leafRule;

    bool setCurrentFileName(const QString &fileName, int ls, int type = 0); //ls 0 - load, ls 1 = save, type 0 - fileNameMain, type 1 - fileNameLeaf
};

#endif // MAINWINDOW_H
