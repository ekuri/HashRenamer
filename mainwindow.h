#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_addDirectoriesPushButton_clicked();

    void on_addFilesPushButton_clicked();

    void on_runPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMap<QByteArray, QString> hashRenameMap;

    int runWithHashType(QStringList &filenamesStringList,
                                QCryptographicHash::Algorithm cryptoType = QCryptographicHash::Sha1);
    void setSelectStatus(bool status);
    bool hashWithFileName(const QString &filename, QCryptographicHash::Algorithm cryptoType);
    QStringList getAllFileNamesStringList(const QString &targetName);
};

#endif // MAINWINDOW_H
