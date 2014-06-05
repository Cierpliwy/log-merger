#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QMutex>
#include <map>
#include "TokenGraph.h"
#include "File.h"
#include "FileHelper.h"
#include "NeedlemanWunschAlgorithm.h"
#include "NWASettings.h"

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

    void loadFiles(const std::vector<QString> &fileList, unsigned from, unsigned to);
    std::vector<const File *> calculateFiles(const std::vector<const File*> &fileList,
                                              unsigned from,
                                              unsigned to,
                                              const File &templateFile,
                                              TokenGraph *tokenGraph);

public slots:
    void addFiles();
    void findUniqies();
    void previewUnique(QListWidgetItem*);
    void clearAll();
    void changeSettings(QString settingsName);
    void updateSettings();

private:
    void updateTextEdit(const TokenGraph& graph, QTextEdit& textEdit);

    FileHelper fh;
    std::map<int, File*> files;
    std::map<int, TokenGraph*> uniques;
    NeedlemanWunschAlgorithm nwa;
    Ui::MainWindow *ui;

    // Settings map
    std::map<QString, NWASettings> settings;
    NWASettings currentSettings;

    // Loading files variables
    QMutex loadMutex;
    int maxFiles;
    bool closeLoading;
    int loadedFiles;
    int processedFiles;

    // Calculating variables
    QMutex calculateMutex;
    int passNumber;
    int itemNumber;
    int totalCalculations;
    int usedHeuristicNumber;
    int uniquesNumber;
    bool calculationCanceled;
};

#endif // MAINWINDOW_H
