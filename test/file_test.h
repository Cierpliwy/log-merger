#ifndef FILE_TEST_H
#define FILE_TEST_H
#include <QtTest/QTest>
#include "../src/File.h"

class TestFile : public QObject, public File
{
    Q_OBJECT

public:
    TestFile(FileHelper &fh) : File(fh), fh(fh) {

    }

private slots:
    void testTokenization();

protected:
    FileHelper &fh;
};

#endif // FILE_TEST_H
