#include "FileHelper.h"
#include "File.h"
using namespace std;

FileHelper::FileHelper()
{
    for(int i = 0; i < 256; ++i) {
        m_stringChar[i] = (isalnum(i) || i == '.' || i == '_');
        m_signChar[i] = (ispunct(i) && i != '.' && i != '_');
        m_numberChar[i] = (isxdigit(i) || i == '.');
    }
    m_freeFileID = 0;
}

long FileHelper::getUniqueID(const std::string &str)
{
    pair<set<string>::iterator,bool> it = m_strings.insert(str);
    return reinterpret_cast<long>(&(*it.first)[0]);
}

void FileHelper::addKeyword(const std::string &str)
{
    m_keywords.insert(str);
}

bool FileHelper::isKeyword(const std::string &str) const
{
    return m_keywords.find(str) != m_keywords.end();
}

bool FileHelper::isNumberChar(char c, int pos) const
{
    if (pos == 1 && c == 'x') return true;
    return m_numberChar[static_cast<int>(c)];
}

bool FileHelper::isStringChar(char c) const
{
    return m_stringChar[static_cast<int>(c)];
}

bool FileHelper::isSignChar(char c) const
{
    return m_signChar[static_cast<int>(c)];
}

int FileHelper::registerFile(const File &file)
{
    int id = m_freeFileID;
    m_files[id] = &file;
    m_freeFileID++;
    return id;
}

void FileHelper::unregisterFile(const File &file)
{
    m_files.erase(file.getID());
}
