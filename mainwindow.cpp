#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

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

void MainWindow::openPcap(const char *path)
{
    this->closePcap();
    this->m_pcaploader = new pcap_loader(path);
    ui->tableView->setModel(new PcapPackTableModel(m_pcaploader));
    this->setWindowTitle(QString(WINDOW_TITLE)+ " - " + path );
}

void MainWindow::closePcap()
{
    if ( this->m_pcaploader != NULL )
    {
        delete this->m_pcaploader;
        this->m_pcaploader = NULL;
    }
    ui->tableView->setModel(new PcapPackTableModel(m_pcaploader));
    this->setWindowTitle(WINDOW_TITLE);
}

void MainWindow::initView()
{
    //添加菜单栏事件
    QObject::connect( ui->actionOpen, SIGNAL(triggered()), this,SLOT(openFile()) );
    QObject::connect( ui->actionExport, SIGNAL(triggered()), this,SLOT(exportFile()) );
    QObject::connect( ui->actionClose_File, SIGNAL(triggered()), this,SLOT(closeFile()) );
    QObject::connect( ui->actionExit, SIGNAL(triggered()), this, SLOT(close()) );

    //初始化listView
    ui->tableView->setModel(new PcapPackTableModel(m_pcaploader));

    //设置Title
    this->setWindowTitle(WINDOW_TITLE);
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this);
    if(path.isEmpty()) return;
    this->openPcap(path.toLocal8Bit().data());
}

void MainWindow::exportFile()
{

}

void MainWindow::closeFile()
{
    this->closePcap();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    vector<unsigned char> data;
    if(index.row() >= this->m_pcaploader->get_packs_count()) return;
    pcap_data_info info = this->m_pcaploader->get_pack(index.row());
    data.resize(info.length());
    this->m_pcaploader->read_from_pack(info,data.data(),data.size());

    QString text = (char*)data.data();
    ui->outputText->setText(text);

    QString hex;

    {
        int i;
        for(i=0;i<data.size();++i)
        {
            hex += QString().sprintf("%02X ", data.at(i));
            if( (i+1) % 16 == 0 )
            {
                hex += "\n";
            }
        }
    }

    ui->outputHex->setText(hex);
}
