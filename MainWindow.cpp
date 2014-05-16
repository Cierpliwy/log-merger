#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTime>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <queue>
#include <QRegExp>
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

    connect(ui->addFolderButton, SIGNAL(clicked()), this, SLOT(addFiles()));
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(findUniqies()));
    connect(ui->uniquesList, SIGNAL(itemActivated(QListWidgetItem*)),
            this, SLOT(previewUnique(QListWidgetItem*)));
    connect(ui->clearAllPushButton, SIGNAL(clicked()), this, SLOT(clearAll()));
}

MainWindow::~MainWindow()
{
    clearAll();
    delete ui;
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
    QString directoryPath = QFileDialog::getExistingDirectory();
    queue<QDir> m_dirs;
    if (directoryPath.size() > 0) m_dirs.push(QDir(directoryPath));

    QRegExp regExp(ui->patternEdit->text());
    regExp.setPatternSyntax(QRegExp::Wildcard);
    QProgressDialog progress("", "Cancel", 0, 100, this);
    progress.setWindowTitle("Tokenizing files...");
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.show();

    while (!m_dirs.empty()) {
        QDir &dir = m_dirs.front();
        dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList list = dir.entryInfoList();
        progress.setMaximum(list.size());
        QTime timer;
        timer.start();

        for(int i = 0; i < list.size(); ++i) {
            if (progress.wasCanceled()) {
                break;
            }
            if (list[i].isDir()) {
                if (ui->recursiveCheckBox->isChecked())
                    m_dirs.push(QDir(list[i].absoluteFilePath()));
                continue;
            }
            if (!regExp.exactMatch(list[i].fileName())) continue;
            if (timer.elapsed() > 100) {
                progress.setValue(i);
                progress.setLabelText(QString("Tokenizing: %1").arg(list[i].absoluteFilePath()));
                timer.restart();
            }
            QString filePath = list[i].absoluteFilePath();
            File *file = new File(fh);
            file->load(filePath.toStdString());
            if (file->getSize() > 10000) {
                delete file;
                continue;
            }
            file->tokenize();
            if (file->getTokens().size() == 0) {
                delete file;
                continue;
            }
            files[file->getID()] = file;
            ui->fileListWidget->addItem(file->getName().c_str());
        }

        m_dirs.pop();
        if (m_dirs.empty()) progress.setValue(list.size());
    }

    ui->fileLabel->setText(QString("%1").arg(files.size()));
}

void MainWindow::findUniqies()
{
    for(auto uniquePair : uniques) {
        delete uniquePair.second;
    }
    uniques.clear();
    ui->uniquesList->clear();

    QProgressDialog progress("", "Cancel", 0, files.size(), this);
    progress.setWindowTitle("Uniques");
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(0);
    progress.show();

    QTime timer, timer2;
    timer.start();
    timer2.start();

    std::multimap<float, File*> map1, map2;
    for(auto &f : files) {
        map1.insert(pair<float,File*>(1,f.second));
    }

    int uniquesNum = 0;
    int heur = 0;
    while(!map1.empty()) {
        uniquesNum++;
        if (timer.elapsed() > 10) {
            progress.setValue(files.size()-map1.size());
            timer.restart();
        }
        if (progress.wasCanceled()) {
            break;
        }

        File* templateFile = map1.begin()->second;
        TokenGraph *graph = new TokenGraph;

        int i = 0;
        for (auto &f : map1) {
            progress.setLabelText(
                        QString("Finding uniques: %1 (%2/%3)").
                        arg(uniquesNum).arg(i).arg(map1.size()));

            if (f.second != templateFile) {
                if (ui->useHeuristicCheckBox->isChecked() &&
                    !nwa.isWorthCalculating(*templateFile, *f.second,
                                            ui->similaritySpinBox->value() / 100.0f)) {
                    map2.insert(pair<float,File*>(nwa.getScore(), f.second));
                    heur++;
                } else {
                    nwa.calculate(*templateFile, *f.second);
                    if (nwa.getScore() > ui->similaritySpinBox->value() / 100.0f)
                        nwa.updateTokenGraph(*graph);
                    else
                        map2.insert(pair<float,File*>(nwa.getScore(), f.second));
                }
            }
            i++;
        }

        if (graph->empty()) nwa.prepareTokenGraph(*graph, *templateFile);

        uniques[templateFile->getID()] = graph;
        ui->uniquesList->addItem(templateFile->getName().c_str());

        map1 = map2;
        map2.clear();
    }
    progress.setValue(files.size());
    ui->uniqueLabel->setText(QString("%1").arg(uniques.size()));
    qDebug() << "Total time" << timer2.elapsed() << " ms, Heur: " << heur;
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
}
