#ifndef NWA_TEST_H
#define NWA_TEST_H
#include <QtTest/QTest>
#include "../src/NeedlemanWunschAlgorithm.h"
#include "../src/FileHelper.h"

class TestNWA : public QObject, public NeedlemanWunschAlgorithm
{
    Q_OBJECT

public:
    TestNWA(FileHelper &fh) : f1(fh), f2(fh) {}

private slots:
    void testCalculation();

private:
    File f1, f2;
};


#endif // NWA_TEST_H
