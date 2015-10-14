#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QXmlStreamWriter>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

    setSelectStatus(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addDirectoriesPushButton_clicked()
{
    ui->selectedFileOrDirectoryLineEdit->setText(QFileDialog::getExistingDirectory());
}

void MainWindow::on_addFilesPushButton_clicked()
{
    ui->selectedFileOrDirectoryLineEdit->setText(QFileDialog::getOpenFileName());
}

void MainWindow::on_runPushButton_clicked()
{
    QString targetName = ui->selectedFileOrDirectoryLineEdit->text();
    if (targetName.length() <= 0) {
        return;
    }

    setSelectStatus(false);

    QStringList filenamesStringList = getAllFileNamesStringList(targetName);
    qDebug() << "Files count: " << filenamesStringList.size();

    if (ui->sha1RadioButton->isChecked()) {
        runWithHashType(filenamesStringList, QCryptographicHash::Sha1);
    } else if (ui->md5RadioButton->isChecked()) {
        runWithHashType(filenamesStringList, QCryptographicHash::Md5);
    }

    setSelectStatus(true);
}

int MainWindow::runWithHashType(QStringList &filenamesStringList,
                                        QCryptographicHash::Algorithm cryptoType)
{

    int filesCount = filenamesStringList.size();

    while (filenamesStringList.size()) {
        QFileInfo fileInfo(filenamesStringList.front());
        filenamesStringList.pop_front();

        if (!fileInfo.exists()) {
            qDebug() << "Invalid path: " << fileInfo.absoluteFilePath();
            continue;

        } else if (fileInfo.isFile()) {
            QApplication::processEvents();
            ui->statusLabel->setText(fileInfo.absoluteFilePath());
            ui->progressBar->setValue((int)((filesCount - filenamesStringList.size()) * 1.0 / filesCount * 100));
            if (!hashWithFileName(fileInfo.absoluteFilePath(), cryptoType)) {
                qDebug() << "rename failed";
            }
        }
    }

    qDebug() << "finished";
    return 0;
}

// when runing, the status is false, otherwise the status is true
void MainWindow::setSelectStatus(bool status)
{
    if (!status) {
        ui->progressBar->setValue(0);
    } else {
        ui->statusLabel->setText("");
        ui->selectedFileOrDirectoryLineEdit->setText("");
    }

    ui->runPushButton->setVisible(status);
    ui->pausePushButton->setVisible(!status);
    ui->stopPushButton->setVisible(!status);
    ui->progressBar->setVisible(!status);
    ui->sha1RadioButton->setEnabled(status);
    ui->md5RadioButton->setEnabled(status);
    ui->addDirectoriesPushButton->setEnabled(status);
    ui->addFilesPushButton->setEnabled(status);
    ui->selectedFileOrDirectoryLineEdit->setEnabled(status);
}

bool MainWindow::hashWithFileName(const QString &filename, QCryptographicHash::Algorithm cryptoType)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray fileData = file.readAll();
    QByteArray cryptoResult = QCryptographicHash::hash(fileData, cryptoType).toHex();


    QFileInfo fileInfo(filename);
    QString resultFileName = cryptoResult;
    if (fileInfo.suffix().length() > 0) {
        resultFileName += "." + fileInfo.suffix();
    }

    if (fileInfo.fileName() == resultFileName) {
        file.close();
        return true;
    }
    QString absoluteResultFileName = fileInfo.absolutePath() + "/" + resultFileName;


    bool isRenameSuccess = true;
    if (QFile::exists(absoluteResultFileName)) {
        file.remove();
    } else {
        isRenameSuccess = file.rename(absoluteResultFileName);
    }
    file.close();
    return isRenameSuccess;
}

QStringList MainWindow::getAllFileNamesStringList(const QString &targetName)
{
    QStringList filenamesStringList;
    QStringList directoryStringList;
    QFileInfo fileInfo(targetName);

    // targetName is neither a file nor a directory
    if (!fileInfo.exists()) {
        return filenamesStringList;
    }

    // targetName is a file
    if (fileInfo.isFile()) {
        //qDebug() << targetName;
        filenamesStringList.push_back(targetName);
        return filenamesStringList;
    }

    // targetName is a directory, recursively finds files
    if (fileInfo.isDir()) {

        directoryStringList.push_back(targetName);
        while (directoryStringList.size()) {

            QDir currentDirectory(directoryStringList.front());
            directoryStringList.pop_front();

            currentDirectory.setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
            QStringList targetUnderCurrentDirectory = currentDirectory.entryList();
            foreach (QString s, targetUnderCurrentDirectory) {
                //qDebug() << currentDirectory.absoluteFilePath(s);
                directoryStringList += currentDirectory.absoluteFilePath(s);
            }

            currentDirectory.setFilter(QDir::NoDotAndDotDot | QDir::Files);
            targetUnderCurrentDirectory = currentDirectory.entryList();

            foreach (QString s, targetUnderCurrentDirectory) {
                filenamesStringList += currentDirectory.absoluteFilePath(s);
            }
        }
    }

    return filenamesStringList;
}
