#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pcappacktablemodel.h"
#include "pcap_loader.h"

#define WINDOW_TITLE "PcapReader"

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
    void openPcap(const char* path);
    void closePcap();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void openFile();
    void closeFile();
    void exportFile();
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    pcap_loader* m_pcaploader;
    PcapPackTableModel* m_model;
};

#endif // MAINWINDOW_H
