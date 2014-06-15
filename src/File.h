#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <vector>
#include "Token.h"

class FileHelper;

/**
 * @brief Podstawowa klasa reprezentująca plik. Umożliwia ona rozbiór pliku na
 * części w postaci tokenów. Reprezentacja ta dalej służy do porównywania dwóch
 * plików.
 */
class File
{
public:
    File(FileHelper& fileHelper) : m_fileHelper(fileHelper) {}
    ~File();

    bool load(const std::string& filePath);
    bool loadFromString(const std::string &fileName, const std::string &content);
    void tokenize();

    const std::string& getName() const {
        return m_name;
    }

    const std::vector<Token>& getTokens() const {
        return m_tokens;
    }

    const Token& operator [] (size_t i) const {
        return m_tokens[i];
    }

    FileHelper& getHelper() const {
        return m_fileHelper;
    }

    unsigned getSize() const {
        return m_data.size();
    }

    int getID() const {
        return m_id;
    }

protected:

    bool isTokenCompleted(char c);
    void addToToken(char c);
    Token::TokenType getTokenType(char c);
    void closeToken();

    FileHelper &m_fileHelper;
    std::string m_data;
    std::string m_name;
    std::vector<Token> m_tokens;
    Token m_tmpToken;
    int m_id;
};

#endif // FILE_H
