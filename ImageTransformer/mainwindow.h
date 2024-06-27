#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QBuffer>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include "method.h"
#include "convert.h"
#include "stc.h"
#include "rnam.h"
#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    method *p;
private slots:
    void openActionSlot();
    void saveActionSlot();
    void saveAction2Slot();
    void on_action_4_clicked();
    void on_action_5_clicked();
    void on_cin1_editingFinished();
    void on_cin2_editingFinished();
    void on_cin3_editingFinished();
    void on_cin4_editingFinished();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_radioButton_6_clicked();
    void on_radioButton_7_clicked();
    void on_radioButton_8_clicked();
    void on_pushButton_QPushButton();
private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
