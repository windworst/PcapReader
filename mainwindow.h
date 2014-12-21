#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pcap_loader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void initView();
    void initTableView();
    void openPcap(const char* path);
    void closePcap();

private slots:
    void openFile();
    void closeFile();
    void exportFile();

private:
    Ui::MainWindow *ui;

    pcap_loader* m_pcaploader;
};

#endif // MAINWINDOW_H
