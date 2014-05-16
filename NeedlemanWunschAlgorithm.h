#ifndef NEEDLEMANWUNSCHALGORITHM_H
#define NEEDLEMANWUNSCHALGORITHM_H
#include "TokenGraph.h"
#include "File.h"

class NeedlemanWunschAlgorithm
{
public:
    NeedlemanWunschAlgorithm();
    ~NeedlemanWunschAlgorithm();

    bool calculate(const File& file1, const File& file2);
    void prepareTokenGraph(TokenGraph &graph, const File& file);
    void updateTokenGraph(TokenGraph &graph);
    bool isWorthCalculating(const File& file1, const File& file2,
                            float similarity);

    float getScore() const {
        return m_score;
    }

private:

    void free();
    void insertTokenInGraph(TokenGraph &graph, unsigned i,
                            Token token, int fileID);
    float getMaximalScore(const File& file);

    struct Cell {
        Cell() : score(0), dx(0), dy(0) {}
        Cell(int score, int dx, int dy) :
            score(score), dx(dx), dy(dy) {}

        int score;
        int dx;
        int dy;
    };

    Cell **m_tab;
    unsigned m_tabSize;
    const File *m_file1, *m_file2;
    float m_score;
    int m_correctScore[Token::TOKEN_NUM][Token::TOKEN_NUM];
    int m_incorrectScore[Token::TOKEN_NUM][Token::TOKEN_NUM];
    int m_skipScore;
};

#endif // NEEDLEMANWUNSCHALGORITHM_H
