#include <Windows.h>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    char buf[256];
    DWORD dwSize = GetLogicalDriveStringsA(sizeof(buf),buf);

    if(dwSize > 0 && dwSize <= sizeof(buf)){
        char*p = buf;
        while(*p){
            ui->listWidget->addItem(QString(*p) + ":\\");
            p += strlen(p) + 1;
        }
    }
    ui->textBrowser->setText(QString("Total storages count: %1").arg(dwN));
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_refreshButton_clicked);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onListWidgetItemClicked);
}


void MainWindow::onListWidgetItemClicked(QListWidgetItem* item)
{
    QString dwInfo;
    QString dwName = item->text();

    DWORD sectorsPerCluster;
    DWORD bytesPerSector;
    DWORD freeClusters;
    DWORD totalClusters;
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalNumberOfBytes;
    ULARGE_INTEGER totalNumberOfFreeBytes;

    wchar_t volumeName[MAX_PATH + 1] = { 0 };
    wchar_t fileSystem[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD fileSystemType = 0;

    bool result = GetDiskFreeSpaceA(
        dwName.toStdString().c_str(),
        &sectorsPerCluster,
        &bytesPerSector,
        &freeClusters,
        &totalClusters);

    UINT driveType = GetDriveType(dwName.toStdWString().c_str());
    switch (driveType) {
        case DRIVE_REMOVABLE:
            dwInfo += "Drive is a removable disk";
            break;
        case DRIVE_FIXED:
            dwInfo += "Drive is a fixed disk";
            break;
        case DRIVE_REMOTE:
            dwInfo += "Drive is a remote (network) drive";
            break;
        case DRIVE_CDROM:
            dwInfo += "Drive is a CD-ROM drive";
            break;
        case DRIVE_RAMDISK:
            dwInfo += "Drive is a RAM disk";
            break;
        default:
            dwInfo += "Unknown drive type";
    }
    dwInfo += "\n\n";

    if(result){
        dwInfo += "\tClusters info\n";
        dwInfo += QString("Sectors per cluster\t: %1\n").arg(sectorsPerCluster);
        dwInfo += QString("Bytes per sector\t: %1 bytes\n").arg(bytesPerSector);
        dwInfo += QString("Free clusters\t\t: %1\n").arg(freeClusters);
        dwInfo += QString("Total clusters\t\t: %1\n").arg(totalClusters);
        dwInfo += "\n";
    }
    else{
        dwInfo += "Error getting disk clusters information\n\n";
    }

    result = GetDiskFreeSpaceEx(
                dwName.toStdWString().c_str(),
                &freeBytesAvailable,
                &totalNumberOfBytes,
                &totalNumberOfFreeBytes
                );
    if(result){
        dwInfo += "\tStorage space info\n";
        dwInfo += QString("Free bytes avalaible\t: %1 bytes\n").arg(static_cast<long long>(freeBytesAvailable.QuadPart));
        dwInfo += QString("\t\t: %1 kb\n").arg(static_cast<long long>(freeBytesAvailable.QuadPart)/1024);
        dwInfo += QString("\t\t: %1 Mb\n").arg(static_cast<long long>(freeBytesAvailable.QuadPart)/1024/1024);
        dwInfo += QString("\t\t: %1 Gb\n").arg(static_cast<long long>(freeBytesAvailable.QuadPart)/1024/1024/1024);
        dwInfo += QString("Total number of bytes\t: %1 bytes\n").arg(static_cast<long long>(totalNumberOfBytes.QuadPart));
        dwInfo += QString("\t\t: %1 kb\n").arg(static_cast<long long>(totalNumberOfBytes.QuadPart)/1024);
        dwInfo += QString("\t\t: %1 Mb\n").arg(static_cast<long long>(totalNumberOfBytes.QuadPart)/1024/1024);
        dwInfo += QString("\t\t: %1 Gb\n").arg(static_cast<long long>(totalNumberOfBytes.QuadPart)/1024/1024/1024);
        /*dwInfo += QString("Total number of free bytes\t: %1 bytes\n").arg(static_cast<long long>(totalNumberOfFreeBytes.QuadPart));
        dwInfo += QString("\t\t: %1 kb\n").arg(static_cast<long long>(totalNumberOfFreeBytes.QuadPart)/1024);
        dwInfo += QString("\t\t: %1 Mb\n").arg(static_cast<long long>(totalNumberOfFreeBytes.QuadPart)/1024/1024);
        dwInfo += QString("\t\t: %1 Gb\n").arg(static_cast<long long>(totalNumberOfFreeBytes.QuadPart)/1024/1024/1024);
        */
        dwInfo += "\n";
    }
    else{
        dwInfo += "Error getting disk space information\n\n";
    }

    result = GetVolumeInformationW(
            dwName.toStdWString().c_str(),
            volumeName,
            sizeof(volumeName),
            &serialNumber,
            NULL,
            &fileSystemType,
            fileSystem,
            sizeof(fileSystem));

    if(result){
        dwInfo += "\tAdditional info\n";
        dwInfo += QString("Volume name\t\t: %1\n").arg(QString::fromWCharArray(volumeName));
        dwInfo += QString("Serial number\t\t: %1\n").arg(serialNumber);
        dwInfo += QString("File system type\t: %1\n").arg(fileSystemType);
        dwInfo += QString("File system name\t: %1\n").arg(fileSystem);
        dwInfo += '\n';
    }

    ui->textBrowser->setText(dwInfo);

}

void MainWindow::on_refreshButton_clicked(){
    ui->listWidget->clear();
    ui->textBrowser->clear();

    char buf[256];
    DWORD dwSize = GetLogicalDriveStringsA(sizeof(buf),buf);

    QString dwList[256];
    int dwN=0;
    if(dwSize > 0 && dwSize <= sizeof(buf)){
        char*p = buf;
        while(*p){
            ui->listWidget->addItem(QString(*p) + ":\\");
            dwList[dwN] = QString(p);
            p += strlen(p) + 1;
            dwN++;
        }
    }
    ui->textBrowser->setText(QString("Total storages count: %1").arg(dwN));
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onListWidgetItemClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

