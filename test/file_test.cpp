#include "file_test.h"
#include "../src/FileHelper.h"
#include <set>
using namespace std;

void TestFile::testTokenization() {

    // Create a file to test tokens
    m_data = "0 0.09 0x34 .09 31ala Ma ko-ta Ala alA ala\n?";
    m_name = "Ala";
    fh.registerFile(*this);
    fh.addKeyword("Ala");
    fh.addKeyword("alA");

    // Tokenize file and check it's tokens types and values
    tokenize();
    QVERIFY(m_tokens.size() == 14);
    QVERIFY(m_tokens[0].type == Token::NUMBER);
    QVERIFY(m_tokens[0].value == "0");
    QVERIFY(m_tokens[1].type == Token::NUMBER);
    QVERIFY(m_tokens[1].value == "0.09");
    QVERIFY(m_tokens[2].type == Token::NUMBER);
    QVERIFY(m_tokens[2].value == "0x34");
    QVERIFY(m_tokens[3].type == Token::NUMBER);
    QVERIFY(m_tokens[3].value == ".09");
    QVERIFY(m_tokens[4].type == Token::STRING);
    QVERIFY(m_tokens[4].value == "31ala");
    QVERIFY(m_tokens[5].type == Token::STRING);
    QVERIFY(m_tokens[5].value == "Ma");
    QVERIFY(m_tokens[6].type == Token::STRING);
    QVERIFY(m_tokens[6].value == "ko");
    QVERIFY(m_tokens[7].type == Token::SIGN);
    QVERIFY(m_tokens[7].value == "-");
    QVERIFY(m_tokens[8].type == Token::STRING);
    QVERIFY(m_tokens[8].value == "ta");
    QVERIFY(m_tokens[9].type == Token::KEYWORD);
    QVERIFY(m_tokens[9].value == "Ala");
    QVERIFY(m_tokens[10].type == Token::KEYWORD);
    QVERIFY(m_tokens[10].value == "alA");
    QVERIFY(m_tokens[11].type == Token::STRING);
    QVERIFY(m_tokens[11].value == "ala");
    QVERIFY(m_tokens[12].type == Token::NEW_LINE);
    QVERIFY(m_tokens[12].value == "\n");
    QVERIFY(m_tokens[13].type == Token::SIGN);
    QVERIFY(m_tokens[13].value == "?");

    // Check if IDs are unique
    set<int> idSet;
    for (unsigned  i = 0; i < m_tokens.size(); ++i) {
        QVERIFY(idSet.insert(m_tokens[i].id).second == true);
    }
}
