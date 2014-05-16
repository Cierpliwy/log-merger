#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <map>
#include <set>
#include <string>

class File;

/**
 * @brief Klasa pomocnicza do obróbki plików. Dla pewnego zbioru plików
 * ustalane są unikalne numery id dla wartości tokenów, wyszukiwany jest
 * maksymalny rozmiar pliku przerabianego oraz dostępne są funkcje
 * przyspieszające parsowanie.
 */
class FileHelper
{
public:
    FileHelper();

    long getUniqueID(const std::string &str);
    void addKeyword(const std::string &str);
    bool isKeyword(const std::string &str) const;
    bool isStringChar(char c) const;
    bool isNumberChar(char c, int pos) const;
    bool isSignChar(char c) const;
    int registerFile(const File& file);
    void unregisterFile(const File& file);
    const std::map<int, const File*> getFiles() const {
        return m_files;
    }

private:

    std::set<std::string> m_strings;
    std::set<std::string> m_keywords;
    bool m_stringChar[256];
    bool m_numberChar[256];
    bool m_signChar[256];
    int m_freeFileID;
    std::map<int, const File*> m_files;
};

#endif // FILEHELPER_H
