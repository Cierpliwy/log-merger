#include "File.h"
#include "FileHelper.h"
#include <fstream>
using namespace std;

bool File::load(const std::string &filePath)
{
    ifstream is(filePath.c_str(), ios::in | ios::binary);
    if (is) {
        is.seekg(0, ios::end);
        m_data.resize(is.tellg());
        is.seekg(0, ios::beg);
        is.read(&m_data[0], m_data.size());
        m_name = filePath;
        is.close();
        m_id = m_fileHelper.registerFile(*this);
        return true;
    }
    return false;
}

bool File::loadFromString(const std::string &fileName, const std::string &content)
{
    m_data = content;
    m_name = fileName;
    m_id = m_fileHelper.registerFile(*this);
    return true;
}

File::~File()
{
    m_fileHelper.unregisterFile(*this);
}

void File::tokenize()
{
    m_tmpToken = Token();
    for(unsigned i = 0; i < m_data.size(); ++i) {
        char c = m_data[i];
        if (isTokenCompleted(c)) closeToken();
        if (isgraph(c) || c == '\n') addToToken(c);
    }
    closeToken();
}

bool File::isTokenCompleted(char c)
{
    if (m_tmpToken.value.empty()) return false;
    if (!isprint(c)) return true;

    char lastChar = m_tmpToken.value[m_tmpToken.value.length() - 1];
    if (m_fileHelper.isStringChar(lastChar) !=
        m_fileHelper.isStringChar(c)) return true;
    if (m_fileHelper.isSignChar(lastChar) !=
        m_fileHelper.isSignChar(c)) return true;
    return false;
}

Token::TokenType File::getTokenType(char c)
{
    if (c == '\n') return Token::NEW_LINE;
    if (m_tmpToken.type == Token::NUMBER &&
        m_fileHelper.isNumberChar(c, m_tmpToken.value.length()))
        return Token::NUMBER;
    if (m_fileHelper.isStringChar(c)) return Token::STRING;
    return Token::SIGN;
}

void File::addToToken(char c)
{
    m_tmpToken.type = getTokenType(c);
    m_tmpToken.value += c;
}

void File::closeToken()
{
    if (m_tmpToken.value.empty()) return;
    m_tmpToken.id = m_fileHelper.getUniqueID(m_tmpToken.value);
    if (m_fileHelper.isKeyword(m_tmpToken.value))
        m_tmpToken.type = Token::KEYWORD;

    m_tokens.push_back(m_tmpToken);
    m_tmpToken = Token();
}
