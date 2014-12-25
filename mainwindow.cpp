#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_pcaploader = NULL;
    this->m_model = new PcapPackTableModel;

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
    this->m_model->set_loader(this->m_pcaploader = new pcap_loader(path));
    ui->tableView->setModel(NULL);
    ui->tableView->setModel(this->m_model);
    ui->tableView->resizeColumnToContents(0);
    if ( this->m_pcaploader->status() == pcap_loader::FILE_TYPE_ERROR )
    {
        QMessageBox::warning(this, "Warning", "File type error");
    }
    else if ( this->m_pcaploader->status() == pcap_loader::OPEN_FILE_ERROR )
    {
        QMessageBox::warning(this, "Warning", "Open file error");
    }
    else
    {
        this->setWindowTitle(QString(WINDOW_TITLE)+ " - " + path );
    }
}

void MainWindow::closePcap()
{
    this->m_model->set_loader(NULL);
    if ( this->m_pcaploader != NULL )
    {
        delete this->m_pcaploader;
        this->m_pcaploader = NULL;
    }
    this->setWindowTitle(WINDOW_TITLE);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        if (event->mimeData()->urls().count()==1)
        {
            event->accept();
            return;
        }
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        if (event->mimeData()->urls().count()==1)
        {
            QString path = event->mimeData()->urls().at(0).toLocalFile();
            this->openPcap(path.toLocal8Bit().data());
            return;
        }
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
    ui->tableView->setModel(this->m_model);
    ui->tableView->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->tableView->resizeColumnToContents(0);
    QAction *a_all,*a_none,*a_reverse;
    ui->tableView->addAction(a_all = new QAction(QString("Select All"),ui->tableView));
    ui->tableView->addAction(a_none = new QAction(QString("unSelect All"),ui->tableView));
    ui->tableView->addAction(a_reverse = new QAction(QString("Reverse Select"),ui->tableView));
    QObject::connect(a_all,SIGNAL(triggered()),this->m_model,SLOT(selectAll()));
    QObject::connect(a_none,SIGNAL(triggered()),this->m_model,SLOT(unselectAll()));
    QObject::connect(a_reverse,SIGNAL(triggered()),this->m_model,SLOT(reverseSelect()));

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
    PcapPackTableModel *model = (PcapPackTableModel *)ui->tableView->model();
    if(model == NULL) return;
    vector<bool> checked_list = model->get_checked_list();
    int check_count = 0;

    {
        unsigned int i;
        for(i=0; i<checked_list.size(); ++i)
        {
            if( checked_list.at(i) )
            {
                ++ check_count;
            }
        }
    }
    if(check_count==0)
    {
        QMessageBox::warning(this,"Warning","Please choice pack to export");
        return;
    }
    QString path = QFileDialog::getSaveFileName(this);
    QDir().mkpath(path);
    unsigned int i;
    for( i=0; i<checked_list.size(); ++i)
    {
        if(checked_list.at(i))
        {
            QString outpath = path + "/" + QString("%1").arg(i+1);
            vector<unsigned char> data;
            pcap_data_info info = this->m_pcaploader->get_pack(i);
            data.resize(info.length());
            this->m_pcaploader->read_from_pack(info,data.data(),data.size());

            QFile file( outpath );
            if ( file.open( QIODevice::WriteOnly ) ) {
                QDataStream stream( &file );
                stream.writeRawData((const char*)data.data(),data.size());
                file.close();
            }
        }
    }
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
        unsigned int i;
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
