/*
 * @Author: Libra
 * @Date: 2025-05-06 14:15:40
 * @LastEditors: Libra
 * @Description: 
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
