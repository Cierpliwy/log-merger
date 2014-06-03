#include "NeedlemanWunschAlgorithm.h"
#include "FileHelper.h"

NeedlemanWunschAlgorithm::NeedlemanWunschAlgorithm() :
    m_correctScore{{ 1,-1, -5,-3, -20},
                   {-1, 2, -3,-2, -20},
                   {-5,-3,  5,-2, -20},
                   {-3,-2, -2, 3, -20},
                  {-20,-20,-20,-20,20}},
    m_incorrectScore{{ 0,-1, -5,-3, -20},
                     {-1, 0, -3,-2, -20},
                     {-5,-3,  -5,-2, -20},
                     {-3,-2, -2, 0, -20},
                    {-20,-20,-20,-20,20}}
{
    m_skipScore = -1;
    m_score = 0;
    m_tab = nullptr;
    m_tabSize = 0;
    m_file1 = nullptr;
    m_file2 = nullptr;
}

NeedlemanWunschAlgorithm::~NeedlemanWunschAlgorithm()
{
    free();
}

void NeedlemanWunschAlgorithm::free()
{
    if (m_tab) {
        for(unsigned i = 0; i < m_tabSize; ++i)
            delete[] m_tab[i];
        delete[] m_tab;
        m_tab = nullptr;
    }
}

float NeedlemanWunschAlgorithm::getMaximalScore(const File &file)
{
    float maxScore = 0;
    for (unsigned i = 0; i < file.getTokens().size(); ++i) {
        int tokenNum = file[i].type;
        maxScore += m_correctScore[tokenNum][tokenNum];
    }
    return maxScore;
}

bool NeedlemanWunschAlgorithm::calculate(const File &file1, const File &file2)
{
    // Check if two files uses the same file helper
    if (&file1.getHelper() != &file2.getHelper()) return false;
    m_file1 = &file1;
    m_file2 = &file2;

    // If necessary alocate new space for an algorithm
    unsigned newSize = std::max(file1.getTokens().size() + 1,
                                file2.getTokens().size() + 1);

    if (newSize > m_tabSize) {
        // Free current memory
        free();
        m_tabSize = newSize;

        // Allocate new memory
        m_tab = new Cell*[m_tabSize];
        for (unsigned i = 0; i < m_tabSize; ++i)
            m_tab[i] = new Cell[m_tabSize];
    }

    // Get file's token numbers
    unsigned f1num = file1.getTokens().size();
    unsigned f2num = file2.getTokens().size();

    // Reset memory
    // for (unsigned x = 0; x < f1num + 1; ++x)
    //     for (unsigned y = 0; y < f2num + 1; ++y)
    //         m_tab[x][y] = Cell();

    // Calculate file 1 max score
    float maxScore = getMaximalScore(file1);

    // Starting penalty
    for (unsigned x = 0; x < f1num + 1; ++x)
        m_tab[x][0] = Cell(m_skipScore*x, -1, 0);
    for (unsigned y = 0; y < f2num + 1; ++y)
        m_tab[0][y] = Cell(m_skipScore*y, 0, -1);

    // Calculate each remaining cell
    int dx, dy, cellCost;

    for (unsigned x = 1; x < f1num + 1; ++x) {
        for (unsigned y = 1; y < f2num + 1; ++y) {
            int tmpCost = m_tab[x][y-1].score + m_skipScore;
            dx = 0;
            dy = -1;
            cellCost = tmpCost;

            tmpCost = m_tab[x-1][y].score + m_skipScore;
            if (tmpCost > cellCost) {
                dx = -1;
                dy = 0;
                cellCost = tmpCost;
            }

            if (file1[x-1] == file2[y-1])
                tmpCost = m_tab[x-1][y-1].score +
                          m_correctScore[file1[x-1].type][file2[y-1].type];
            else
                tmpCost = m_tab[x-1][y-1].score +
                          m_incorrectScore[file1[x-1].type][file2[y-1].type];

            if (tmpCost > cellCost) {
                dx = -1;
                dy = -1;
                cellCost = tmpCost;
            }

            m_tab[x][y] = Cell(cellCost, dx, dy);
        }
    }

    // Last cell contains score
    m_score = std::max(m_tab[f1num][f2num].score, 0) / maxScore;
    return true;
}

void NeedlemanWunschAlgorithm::insertTokenInGraph(TokenGraph &graph,
                                                  unsigned i, Token token,
                                                  int fileID)
{
    if (graph[i].values.empty()) {
        graph[i].firstToken = token;
        TokenGraphValue tgv;
        tgv.token = token;
        tgv.files.insert(fileID);
        graph[i].values[token] = tgv;
    } else {
        const auto& it = graph[i].values.find(token);
        if (it == graph[i].values.end()) {
            TokenGraphValue tgv;
            tgv.token = token;
            tgv.files.insert(fileID);
            graph[i].values[token] = tgv;
        } else {
            it->second.files.insert(fileID);
        }
    }
}

void NeedlemanWunschAlgorithm::prepareTokenGraph(TokenGraph &graph,
                                                 const File &file)
{
    if (graph.empty()) {
        graph.resize(file.getTokens().size() * 2 + 1);
        for (unsigned i = 0; i < file.getTokens().size(); i++) {
            insertTokenInGraph(graph, 2*i+1, file[i], file.getID());
        }
    }
}

void NeedlemanWunschAlgorithm::updateTokenGraph(TokenGraph &graph)
{
    unsigned f1num = m_file1->getTokens().size();
    unsigned f2num = m_file2->getTokens().size();

    prepareTokenGraph(graph, *m_file1);
    if (graph.size() != f1num * 2 + 1) return;

    // Add new tokens from algorithm
    int x = f1num;
    int y = f2num;
    int tokenNum = f1num - 1;
    Token token;

    while(x != 0 && y != 0) {
        Cell cell = m_tab[x][y];

        // Both tokens are compared
        if (cell.dx == -1 && cell.dy == -1) {
            Token token1 = (*m_file1)[x-1];
            Token token2 = (*m_file2)[y-1];

            if (token2.value == "jump") {
                token2.value.begin();
            }

            if (token.id) {
                insertTokenInGraph(graph, 2*tokenNum+2, token,
                                   m_file2->getID());
            }
            insertTokenInGraph(graph, 2*tokenNum+1, token2,
                               m_file2->getID());
            token = Token();
            tokenNum--;

        // File 2 token is skipped
        } else if (cell.dx == -1) {
            if (token.id) {
                insertTokenInGraph(graph, 2*tokenNum+2, token,
                                   m_file2->getID());
                token = Token();
            }
            tokenNum--;

        // File 1 token is skipped
        } else {
            Token token2 = (*m_file2)[y-1];
            if (token2.type == Token::NEW_LINE) token2.value = "NEW_LINE";

            // Generate string
            if (token.id) {
                token2.value.append(" ");
                token2.value.append(token.value);
                token.value = token2.value;
                token.type = Token::STRING;
                token.id = m_file1->getHelper().getUniqueID(token.value);
            } else token = token2;
        }

        x += cell.dx;
        y += cell.dy;
    }

    if (token.id)
        insertTokenInGraph(graph, tokenNum*2, token, m_file2->getID());
}

bool NeedlemanWunschAlgorithm::isWorthCalculating(const File &file1,
                                                  const File &file2,
                                                  float similarity)
{
    // We assume that files will be perfectly matched with some skips
    float maxFile1Score = getMaximalScore(file1);

    // How much skips can we have?
    int diffTokens = std::abs(static_cast<int>(file1.getTokens().size()) -
                              static_cast<int>(file2.getTokens().size()));

    if (maxFile1Score + diffTokens*m_skipScore < maxFile1Score*similarity)
        return false;
    return true;
}
