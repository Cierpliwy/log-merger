#include "nwa_test.h"
#include <iostream>
using namespace std;

void TestNWA::testCalculation()
{
    // Create test files
    f1.loadFromString("f1", "Ala ma 3");
    f2.loadFromString("f2", "Bystra Ola ma 5");
    f1.tokenize();
    f2.tokenize();
    QVERIFY(f1.getTokens().size() == 3);
    QVERIFY(f2.getTokens().size() == 4);

    // Add default settings
    NWASettings defaultSettings;
    defaultSettings.equalMatrix[Token::NUMBER][Token::NUMBER] = 6;
    defaultSettings.unequalVector[Token::NUMBER] = 5;
    defaultSettings.equalMatrix[Token::NUMBER][Token::STRING] = -3;
    defaultSettings.equalMatrix[Token::STRING][Token::NUMBER] = -3;
    defaultSettings.equalMatrix[Token::STRING][Token::STRING] = 10;
    defaultSettings.unequalVector[Token::STRING] = 7;
    defaultSettings.basePenalty = -5;
    defaultSettings.penaltyReduction = 1;
    defaultSettings.minPenalty = -1;
    setSettings(defaultSettings);

    calculate(f1,f2);

    // Result preview
    for (int i = 0; i < 4; ++i) {
        for(int j = 0; j < 5; ++j) {
            if (abs(m_tab[i][j].score) < 10)
                cout << " ";
            cout << (m_tab[i][j].score < 0 ? " " : "  ") << m_tab[i][j].score  << "(" << m_tab[i][j].lastPenalty << ") ";
        }
        cout << endl;
    }

    // Proper matrix value:
    //             Ala   ma   3
    //              S    S    N
    //           0 ~ -5  ~ -9 ~ -12
    // Bystra S     \    \
    //          -5    7  ~  2 ~ -2
    // Ola    S     \    \
    //          -9    2    14 ~  9
    // ma     S     \ |  \    \
    //         -12   -2    12   11
    // 5      N       |     | \
    //         -14   -5     7   17

    // Result:
    //        Ala  ma  3
    // Bystra Ola  ma  5

    // 1st column
    QVERIFY(m_tab[0][0].score == 0);
    QVERIFY(m_tab[0][1].score == -5);
    QVERIFY(m_tab[0][2].score == -9);
    QVERIFY(m_tab[0][3].score == -12);
    QVERIFY(m_tab[0][4].score == -14);

    // 2nd column
    QVERIFY(m_tab[1][0].score == -5);
    QVERIFY(m_tab[1][1].score == 7);
    QVERIFY(m_tab[1][2].score == 2);
    QVERIFY(m_tab[1][3].score == -2);
    QVERIFY(m_tab[1][4].score == -5);

    // 3rd column
    QVERIFY(m_tab[2][0].score == -9);
    QVERIFY(m_tab[2][1].score == 2);
    QVERIFY(m_tab[2][2].score == 14);
    QVERIFY(m_tab[2][3].score == 12);
    QVERIFY(m_tab[2][4].score == 7);

    //4th column
    QVERIFY(m_tab[3][0].score == -12);
    QVERIFY(m_tab[3][1].score == -2);
    QVERIFY(m_tab[3][2].score == 9);
    QVERIFY(m_tab[3][3].score == 11);
    QVERIFY(m_tab[3][4].score == 17);


}
