#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidgetItem>
#include <map>
#include "TokenGraph.h"
#include "File.h"
#include "FileHelper.h"
#include "NeedlemanWunschAlgorithm.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief Główne okno aplikacji.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addFiles();
    void findUniqies();
    void previewUnique(QListWidgetItem*);
    void clearAll();

private:
    void updateTextEdit(const TokenGraph& graph, QTextEdit& textEdit);

    FileHelper fh;
    std::map<int, File*> files;
    std::map<int, TokenGraph*> uniques;
    NeedlemanWunschAlgorithm nwa;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
