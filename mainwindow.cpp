#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_pcaploader = NULL;

    this->initView();
}

MainWindow::~MainWindow()
{
    this->closePcap();
    delete ui;
}

void MainWindow::initTableView()
{
}

void MainWindow::openPcap(const char *path)
{
    this->closePcap();
    this->m_pcaploader = new pcap_loader(path);
}

void MainWindow::closePcap()
{
    if ( this->m_pcaploader != NULL )
    {
        delete this->m_pcaploader;
        this->m_pcaploader = NULL;
    }
}

void MainWindow::initView()
{
    //添加菜单栏事件
    QObject::connect( ui->actionOpen, SIGNAL(triggered()), this,SLOT(openFile()) );
    QObject::connect( ui->actionExport, SIGNAL(triggered()), this,SLOT(exportFile()) );
    QObject::connect( ui->actionClose_File, SIGNAL(triggered()), this,SLOT(closeFile()) );
    QObject::connect( ui->actionExit, SIGNAL(triggered()), this, SLOT(close()) );

    //初始化listView
}

void MainWindow::openFile()
{

}

void MainWindow::exportFile()
{

}

void MainWindow::closeFile()
{

}
