#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTime>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <queue>
#include <QRegExp>
#include <QtConcurrent/QtConcurrentRun>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->previewEdit->setReadOnly(true);
    QFont font;
    font.setPixelSize(10);
    ui->previewEdit->setFont(font);

    // Tmp keywords
    fh.addKeyword("INTERNAL");
    fh.addKeyword("Test");
    fh.addKeyword("started");
    fh.addKeyword("finished");
    fh.addKeyword("Check");
    fh.addKeyword("TEST");
    fh.addKeyword("SUCCESSFUL");
    fh.addKeyword("Closing");
    fh.addKeyword("Msg");
    fh.addKeyword("Injection");
    fh.addKeyword("Retries");
    fh.addKeyword("kernelinjector");
    fh.addKeyword("Oops");
    fh.addKeyword("Bug");
    fh.addKeyword("CPU");
    fh.addKeyword("PID");
    fh.addKeyword("Comm");
    fh.addKeyword("Call");
    fh.addKeyword("Code");

    // Add default settings
    NWASettings defaultSettings;
    defaultSettings.equalMatrix[Token::NUMBER][Token::NUMBER] = 100;
    defaultSettings.unequalVector[Token::NUMBER] = 0;
    defaultSettings.equalMatrix[Token::NUMBER][Token::STRING] = -300;
    defaultSettings.equalMatrix[Token::NUMBER][Token::KEYWORD] = -500;
    defaultSettings.equalMatrix[Token::NUMBER][Token::SIGN] = -300;
    defaultSettings.equalMatrix[Token::NUMBER][Token::NEW_LINE] = -500;

    defaultSettings.equalMatrix[Token::STRING][Token::NUMBER] = -300;
    defaultSettings.equalMatrix[Token::STRING][Token::STRING] = 200;
    defaultSettings.unequalVector[Token::STRING] = 0;
    defaultSettings.equalMatrix[Token::STRING][Token::KEYWORD] = -300;
    defaultSettings.equalMatrix[Token::STRING][Token::SIGN] = -200;
    defaultSettings.equalMatrix[Token::STRING][Token::NEW_LINE] = -500;

    defaultSettings.equalMatrix[Token::KEYWORD][Token::NUMBER] = -500;
    defaultSettings.equalMatrix[Token::KEYWORD][Token::STRING] = -300;
    defaultSettings.equalMatrix[Token::KEYWORD][Token::KEYWORD] = 500;
    defaultSettings.unequalVector[Token::KEYWORD] = -500;
    defaultSettings.equalMatrix[Token::KEYWORD][Token::SIGN] = -200;
    defaultSettings.equalMatrix[Token::KEYWORD][Token::NEW_LINE] = -500;

    defaultSettings.equalMatrix[Token::SIGN][Token::NUMBER] = -300;
    defaultSettings.equalMatrix[Token::SIGN][Token::STRING] = -200;
    defaultSettings.equalMatrix[Token::SIGN][Token::KEYWORD] = -200;
    defaultSettings.equalMatrix[Token::SIGN][Token::SIGN] = 300;
    defaultSettings.unequalVector[Token::SIGN] = 0;
    defaultSettings.equalMatrix[Token::SIGN][Token::NEW_LINE] = -500;

    defaultSettings.equalMatrix[Token::NEW_LINE][Token::NUMBER] = -500;
    defaultSettings.equalMatrix[Token::NEW_LINE][Token::STRING] = -500;
    defaultSettings.equalMatrix[Token::NEW_LINE][Token::KEYWORD] = -500;
    defaultSettings.equalMatrix[Token::NEW_LINE][Token::SIGN] = -500;
    defaultSettings.equalMatrix[Token::NEW_LINE][Token::NEW_LINE] = 100;
    defaultSettings.unequalVector[Token::NEW_LINE] = -500;

    defaultSettings.basePenalty = -200;
    defaultSettings.penaltyReduction = 25;
    defaultSettings.minPenalty = -100;

    settings["default"] = defaultSettings;
    changeSettings("default");

    connect(ui->addFolderButton, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(findUniqies()));
    connect(ui->uniquesList, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(previewUnique(QListWidgetItem*)));
    connect(ui->clearAllPushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeSettings(QString)));

    connect(ui->UUSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->USSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->UKSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->UISpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->UNSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->SUSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->SSSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->SKSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->SISpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->SNSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->KUSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->KSSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->KKSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->KISpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->KNSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->IUSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->ISSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->IKSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->IISpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->INSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->NUSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->NSSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->NKSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->NISpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->NNSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->UUSpinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->SSSpinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->KKSpinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->IISpinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->NNSpinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));

    connect(ui->penaltySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->penaltyReductionSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
    connect(ui->minPenaltySpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSettings()));
}

MainWindow::~MainWindow()
{
    clearAll();
    delete ui;
}

void MainWindow::changeSettings(QString settingsName)
{
    currentSettings = settings[settingsName];
    ui->UUSpinBox->setValue(currentSettings.equalMatrix[Token::NUMBER][Token::NUMBER]);
    ui->USSpinBox->setValue(currentSettings.equalMatrix[Token::NUMBER][Token::STRING]);
    ui->UKSpinBox->setValue(currentSettings.equalMatrix[Token::NUMBER][Token::KEYWORD]);
    ui->UISpinBox->setValue(currentSettings.equalMatrix[Token::NUMBER][Token::SIGN]);
    ui->UNSpinBox->setValue(currentSettings.equalMatrix[Token::NUMBER][Token::NEW_LINE]);

    ui->SUSpinBox->setValue(currentSettings.equalMatrix[Token::STRING][Token::NUMBER]);
    ui->SSSpinBox->setValue(currentSettings.equalMatrix[Token::STRING][Token::STRING]);
    ui->SKSpinBox->setValue(currentSettings.equalMatrix[Token::STRING][Token::KEYWORD]);
    ui->SISpinBox->setValue(currentSettings.equalMatrix[Token::STRING][Token::SIGN]);
    ui->SNSpinBox->setValue(currentSettings.equalMatrix[Token::STRING][Token::NEW_LINE]);

    ui->KUSpinBox->setValue(currentSettings.equalMatrix[Token::KEYWORD][Token::NUMBER]);
    ui->KSSpinBox->setValue(currentSettings.equalMatrix[Token::KEYWORD][Token::STRING]);
    ui->KKSpinBox->setValue(currentSettings.equalMatrix[Token::KEYWORD][Token::KEYWORD]);
    ui->KISpinBox->setValue(currentSettings.equalMatrix[Token::KEYWORD][Token::SIGN]);
    ui->KNSpinBox->setValue(currentSettings.equalMatrix[Token::KEYWORD][Token::NEW_LINE]);

    ui->IUSpinBox->setValue(currentSettings.equalMatrix[Token::SIGN][Token::NUMBER]);
    ui->ISSpinBox->setValue(currentSettings.equalMatrix[Token::SIGN][Token::STRING]);
    ui->IKSpinBox->setValue(currentSettings.equalMatrix[Token::SIGN][Token::KEYWORD]);
    ui->IISpinBox->setValue(currentSettings.equalMatrix[Token::SIGN][Token::SIGN]);
    ui->INSpinBox->setValue(currentSettings.equalMatrix[Token::SIGN][Token::NEW_LINE]);

    ui->NUSpinBox->setValue(currentSettings.equalMatrix[Token::NEW_LINE][Token::NUMBER]);
    ui->NSSpinBox->setValue(currentSettings.equalMatrix[Token::NEW_LINE][Token::STRING]);
    ui->NKSpinBox->setValue(currentSettings.equalMatrix[Token::NEW_LINE][Token::KEYWORD]);
    ui->NISpinBox->setValue(currentSettings.equalMatrix[Token::NEW_LINE][Token::SIGN]);
    ui->NNSpinBox->setValue(currentSettings.equalMatrix[Token::NEW_LINE][Token::NEW_LINE]);

    ui->UUSpinBox_2->setValue(currentSettings.unequalVector[Token::NUMBER]);
    ui->SSSpinBox_2->setValue(currentSettings.unequalVector[Token::STRING]);
    ui->KKSpinBox_2->setValue(currentSettings.unequalVector[Token::KEYWORD]);
    ui->IISpinBox_2->setValue(currentSettings.unequalVector[Token::SIGN]);
    ui->NNSpinBox_2->setValue(currentSettings.unequalVector[Token::NEW_LINE]);

    ui->penaltySpinBox->setValue(currentSettings.basePenalty);
    ui->penaltyReductionSpinBox->setValue(currentSettings.penaltyReduction);
    ui->minPenaltySpinBox->setValue(currentSettings.minPenalty);

    nwa.setSettings(currentSettings);
}

void MainWindow::updateSettings()
{
    currentSettings.equalMatrix[Token::NUMBER][Token::NUMBER] = ui->UUSpinBox->value();
    currentSettings.equalMatrix[Token::NUMBER][Token::STRING] = ui->USSpinBox->value();
    currentSettings.equalMatrix[Token::NUMBER][Token::KEYWORD] = ui->UKSpinBox->value();
    currentSettings.equalMatrix[Token::NUMBER][Token::SIGN] = ui->UISpinBox->value();
    currentSettings.equalMatrix[Token::NUMBER][Token::NEW_LINE] = ui->UNSpinBox->value();

    currentSettings.equalMatrix[Token::STRING][Token::NUMBER] = ui->SUSpinBox->value();
    currentSettings.equalMatrix[Token::STRING][Token::STRING] = ui->SSSpinBox->value();
    currentSettings.equalMatrix[Token::STRING][Token::KEYWORD] = ui->SKSpinBox->value();
    currentSettings.equalMatrix[Token::STRING][Token::SIGN] = ui->SISpinBox->value();
    currentSettings.equalMatrix[Token::STRING][Token::NEW_LINE] = ui->SNSpinBox->value();

    currentSettings.equalMatrix[Token::KEYWORD][Token::NUMBER] = ui->KUSpinBox->value();
    currentSettings.equalMatrix[Token::KEYWORD][Token::STRING] = ui->KSSpinBox->value();
    currentSettings.equalMatrix[Token::KEYWORD][Token::KEYWORD] = ui->KKSpinBox->value();
    currentSettings.equalMatrix[Token::KEYWORD][Token::SIGN] = ui->KISpinBox->value();
    currentSettings.equalMatrix[Token::KEYWORD][Token::NEW_LINE] = ui->KNSpinBox->value();

    currentSettings.equalMatrix[Token::SIGN][Token::NUMBER] = ui->IUSpinBox->value();
    currentSettings.equalMatrix[Token::SIGN][Token::STRING] = ui->ISSpinBox->value();
    currentSettings.equalMatrix[Token::SIGN][Token::KEYWORD] = ui->IKSpinBox->value();
    currentSettings.equalMatrix[Token::SIGN][Token::SIGN] = ui->IISpinBox->value();
    currentSettings.equalMatrix[Token::SIGN][Token::NEW_LINE] = ui->INSpinBox->value();

    currentSettings.equalMatrix[Token::NEW_LINE][Token::NUMBER] = ui->NUSpinBox->value();
    currentSettings.equalMatrix[Token::NEW_LINE][Token::STRING] = ui->NSSpinBox->value();
    currentSettings.equalMatrix[Token::NEW_LINE][Token::KEYWORD] = ui->NKSpinBox->value();
    currentSettings.equalMatrix[Token::NEW_LINE][Token::SIGN] = ui->NISpinBox->value();
    currentSettings.equalMatrix[Token::NEW_LINE][Token::NEW_LINE] = ui->NNSpinBox->value();

    currentSettings.unequalVector[Token::NUMBER] = ui->UUSpinBox_2->value();
    currentSettings.unequalVector[Token::STRING] = ui->SSSpinBox_2->value();
    currentSettings.unequalVector[Token::KEYWORD] = ui->KKSpinBox_2->value();
    currentSettings.unequalVector[Token::SIGN] = ui->IISpinBox_2->value();
    currentSettings.unequalVector[Token::NEW_LINE] = ui->NNSpinBox_2->value();

    currentSettings.basePenalty = ui->penaltySpinBox->value();
    currentSettings.penaltyReduction = ui->penaltyReductionSpinBox->value();
    currentSettings.minPenalty = ui->minPenaltySpinBox->value();

    nwa.setSettings(currentSettings);
}


void MainWindow::updateTextEdit(const TokenGraph &graph, QTextEdit &textEdit)
{
    textEdit.clear();
    textEdit.moveCursor(QTextCursor::End);

    // Colors for files
    QString colors[] = {"#3d3474", "#6d3474", "#743434", "#ff4d45",
                        "#c97624", "#bab61d", "#538b36", "#8b3638"};

    QProgressDialog progress("Rendering...", "Cancel", 0, graph.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("Unique file preview");
    progress.setValue(0);
    progress.show();

    for(unsigned i = 0; i < graph.size(); ++i) {
        progress.setValue(i+1);
        QString out;
        unsigned tokenNum = graph[i].values.size();
        if (!tokenNum) continue;
        if (tokenNum > 1) out.append("<font color=grey>(</font>");

        if (graph[i].firstToken.type == Token::NEW_LINE)
            out.append("<br>");
        else {
            if (i % 2 == 1) out.append("<b>");
            if (tokenNum == 1 && graph[i].values.begin()->second.files.size() <= 1)
                out.append("<font color=grey>");
            if (tokenNum > 1) out.append("<font color=blue>");
            out.append(QString("%1").arg(graph[i].firstToken.value.c_str()));
            if (tokenNum > 1) out.append("</font>");
            else out.append(" ");
            if (tokenNum == 1 && graph[i].values.begin()->second.files.size() <= 1)
                out.append("</font>");
            if (i % 2 == 1) out.append("</b>");
        }

        int num = 0;
        for(auto it : graph[i].values) {
            if (it.first != graph[i].firstToken) {
                out.append("<font color=grey>|</font>");
                if (it.first.type == Token::NEW_LINE)
                    out.append("<font color=red><i>NL</i></font>");
                else
                    out.append(QString("<font color=%1>%2</font>").
                               arg(colors[*it.second.files.begin()%8]).
                               arg(it.first.value.c_str()));
            }
            if (num > 4) {
                out.append("<font color=grey>|</font><font color=red>...</font>");
                break;
            }
            num++;
        }

        if (graph[i].values.size() > 1) out.append("<font color=grey>) </font>");
        textEdit.insertHtml(out);
    }
}

void MainWindow::addFiles()
{
    // Get list of all files to tokenize
    QString directoryPath = QFileDialog::getExistingDirectory();
    queue<QDir> m_dirs;
    vector<QString> filePaths;
    if (directoryPath.size() > 0) m_dirs.push(QDir(directoryPath));

    // Apply regexp for files.
    QRegExp regExp(ui->patternEdit->text());
    regExp.setPatternSyntax(QRegExp::Wildcard);

    // Small progress window to show when fetching files.
    QProgressDialog progress("", "Cancel", 0, 0, this);
    progress.setWindowTitle("Fetching files...");
    progress.setWindowModality(Qt::WindowModal);
    progress.show();

    // While there is a folder to explore.
    while (!m_dirs.empty()) {

        // Don't hang GUI thread
        QApplication::processEvents();
        if (progress.wasCanceled()) break;

        // Get content of front directory
        QDir &dir = m_dirs.front();
        dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();

        // For every child
        for(int i = 0; i < list.size(); ++i) {
            // If it's a directory, add it to queue
            if (list[i].isDir()) {
                if (ui->recursiveCheckBox->isChecked())
                    m_dirs.push(QDir(list[i].absoluteFilePath()));
                continue;
            }
            // If it's not matching regexp, ignore it...
            if (!regExp.exactMatch(list[i].fileName())) continue;

            // Save path information
            QString filePath = list[i].absoluteFilePath();
            filePaths.push_back(filePath);
        }
        m_dirs.pop();
    }
    progress.close();

    // Distribute tokenizing between threads
    QProgressDialog progress2("", "Cancel", 0, filePaths.size(), this);
    progress2.setWindowTitle("Tokenizing files...");
    progress2.setWindowModality(Qt::WindowModal);
    progress2.setValue(0);
    progress2.show();
    maxFiles = filePaths.size();
    loadedFiles = 0;
    processedFiles = 0;
    closeLoading = false;

    vector<QFuture<void>> futures;
    if (filePaths.size() < 100) {
        futures.push_back(QtConcurrent::run(this, &MainWindow::loadFiles,
                                            filePaths, 0, filePaths.size()));
    } else {
        // Add more threads for more files.
        unsigned p1 = filePaths.size() / 4;
        unsigned p2 = filePaths.size() / 2;
        unsigned p3 = filePaths.size() * 3 / 4;

        futures.push_back(QtConcurrent::run(this, &MainWindow::loadFiles,
                                            filePaths, 0, p1));
        futures.push_back(QtConcurrent::run(this, &MainWindow::loadFiles,
                                            filePaths, p1, p2));
        futures.push_back(QtConcurrent::run(this, &MainWindow::loadFiles,
                                            filePaths, p2, p3));
        futures.push_back(QtConcurrent::run(this, &MainWindow::loadFiles,
                                            filePaths, p3, filePaths.size()));
    }
    while(true) {
        // If all futures are completed exit.
        unsigned i;
        for (i = 0; i < futures.size(); ++i) {
            if (!futures[i].isFinished()) break;
        }
        if (i == futures.size()) break;

        QApplication::processEvents();
        loadMutex.lock();
        if (progress2.wasCanceled()) {
            closeLoading = true;
            progress2.close();
        }
        progress2.setValue(loadedFiles);
        loadMutex.unlock();
    }
    progress2.setValue(loadedFiles);

    // Update GUI
    ui->fileLabel->setText(QString("%1").arg(processedFiles));
}

// Load file in one thread
void MainWindow::loadFiles(const std::vector<QString> &fileList, unsigned from, unsigned to)
{
    for (unsigned i = from; i < to; ++i) {
        File *file = new File(fh);
        file->load(fileList[i].toStdString().c_str());
        file->tokenize();
        loadMutex.lock();
        loadedFiles++;
        if (file->getTokens().empty() ||
            file->getSize() > 10000) delete file;
        else {
            files[file->getID()] = file;
            processedFiles++;
            //ui->fileListWidget->addItem(fileList[i]);
        }
        if (closeLoading) {
            loadMutex.unlock();
            break;
        }
        loadMutex.unlock();
    }
}

void MainWindow::findUniqies()
{
    for(auto uniquePair : uniques) {
        delete uniquePair.second;
    }
    uniques.clear();
    ui->uniquesList->clear();

    QTime timer;
    timer.start();

    QProgressDialog progress("", "Cancel", 0, files.size(), this);
    progress.setWindowTitle("Uniques");
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.show();

    std::vector<const File*> fileVector;
    for(auto &f : files) {
        fileVector.push_back(f.second);
    }

    unsigned originalSize = fileVector.size();
    passNumber = 0;
    itemNumber = 0;
    totalCalculations = 0;
    usedHeuristicNumber = 0;
    uniquesNumber = 0;
    calculationCanceled = false;

    while(!fileVector.empty()) {

        // Update statistics
        passNumber++;
        itemNumber = 0;

        // First element is a template file
        // Create token graph for it
        const File& templateFile = *fileVector[0];
        TokenGraph *graph = new TokenGraph;

        // Compare it with other files in a vector.
        vector<QFuture<vector<const File*>>> results;

        // Use one thread for small number of comparsions
        if (fileVector.size() < 5) {
             results.push_back(QtConcurrent::run(this, &MainWindow::calculateFiles,
                                                 fileVector, 1, fileVector.size(),
                                                 templateFile, graph));
        } else {
            // Use 4 threads to split a job
            unsigned p1 = (fileVector.size() - 1) / 4;
            unsigned p2 = (fileVector.size() - 1) / 2;
            unsigned p3 = (fileVector.size() - 1) * 3 / 4;
            results.push_back(QtConcurrent::run(this, &MainWindow::calculateFiles,
                                                fileVector, 1, p1,
                                                templateFile, graph));
            results.push_back(QtConcurrent::run(this, &MainWindow::calculateFiles,
                                                fileVector, p1, p2,
                                                templateFile, graph));
            results.push_back(QtConcurrent::run(this, &MainWindow::calculateFiles,
                                                fileVector, p2, p3,
                                                templateFile, graph));
            results.push_back(QtConcurrent::run(this, &MainWindow::calculateFiles,
                                                fileVector, p3, fileVector.size(),
                                                templateFile, graph));
        }

        // Collect results
        while(true) {
            unsigned i;
            for (i = 0; i < results.size(); ++i) {
                if (!results[i].isFinished()) break;
            }
            if (i == results.size()) break;

            calculateMutex.lock();
            if (progress.wasCanceled()) {
                calculationCanceled = true;
                progress.close();
            }
            progress.setValue(originalSize - fileVector.size());
            progress.setLabelText(QString("Total: %1, Pass nr %2: %3/%4. Found uniques: %5")
                                         .arg(totalCalculations).arg(passNumber).arg(itemNumber)
                                         .arg(fileVector.size()-1).arg(uniquesNumber));
            calculateMutex.unlock();
            QApplication::processEvents();
        }

        // Stop everything if calculation was cancelled
        if (calculationCanceled) {
            if (graph) delete graph;
            break;
        }

        // Refresh vector
        fileVector.clear();
        for (unsigned i = 0; i < results.size(); ++i) {
            vector<const File*> newFiles = results[i].result();
            fileVector.insert(fileVector.end(), newFiles.begin(), newFiles.end());
        }

        // Add unique
        uniquesNumber++;
        if (graph->empty()) nwa.prepareTokenGraph(*graph, templateFile);
        uniques[templateFile.getID()] = graph;
        ui->uniquesList->addItem(templateFile.getName().c_str());
    }
    progress.setValue(files.size());
    ui->uniqueLabel->setText(QString("%1").arg(uniques.size()));
    statusBar()->showMessage(QString("Operation finished in %1 ms. Found %2 unique(s) file(s). Heuristic used %3 times: calculations reduced by %4\%.")
                             .arg(timer.elapsed()).arg(uniquesNumber).arg(usedHeuristicNumber).arg((float)usedHeuristicNumber/totalCalculations*100.0f));
}

std::vector<const File*> MainWindow::calculateFiles(const std::vector<const File*> &fileList,
                                unsigned from,
                                unsigned to,
                                const File &templateFile,
                                TokenGraph *tokenGraph)
{
    bool useHeuristics = false;
    float similarity = 0.95;
    vector<const File*> results;

    calculateMutex.lock();
    similarity = ui->similaritySpinBox->value() / 100.0f;
    useHeuristics = ui->useHeuristicCheckBox->isChecked();
    NeedlemanWunschAlgorithm myNWA = nwa;
    calculateMutex.unlock();

    for (unsigned i = from; i < to; ++i) {
        if (useHeuristics && !myNWA.isWorthCalculating(templateFile, *fileList[i], similarity)) {
            results.push_back(fileList[i]);
            calculateMutex.lock();
            usedHeuristicNumber++;
            calculateMutex.unlock();
        } else {
            myNWA.calculate(templateFile, *fileList[i]);
            if (myNWA.getScore() > similarity) {
                calculateMutex.lock();
                myNWA.updateTokenGraph(*tokenGraph);
                calculateMutex.unlock();
            } else {
                results.push_back(fileList[i]);
            }
        }

        calculateMutex.lock();
        itemNumber++;
        totalCalculations++;
        if (calculationCanceled) {
            calculateMutex.unlock();
            break;
        }
        calculateMutex.unlock();
    }

    return results;
}

void MainWindow::previewUnique(QListWidgetItem *item)
{
    QString text = item->text();
    File *file = nullptr;
    for (auto &f : files) {
        if (f.second->getName() == text.toStdString())
            file = f.second;
    }

    if (file) {
        auto it = uniques.find(file->getID());
        if (it != uniques.end()) {
            updateTextEdit(*it->second, *ui->previewEdit);
        }
    }
}

void MainWindow::clearAll()
{
    for(auto filePair : files) {
        delete filePair.second;
    }
    files.clear();
    for(auto uniquePair : uniques) {
        delete uniquePair.second;
    }
    uniques.clear();
    ui->fileListWidget->clear();
    ui->uniquesList->clear();
    ui->fileLabel->setText("0");
    ui->previewEdit->clear();
}
