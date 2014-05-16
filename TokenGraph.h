#ifndef TOKENGRAPH_H
#define TOKENGRAPH_H
#include <string>
#include <vector>
#include <set>
#include <map>
#include "Token.h"

struct TokenGraphValue
{
    Token token;
    std::set<int> files;
};

struct TokenGraphNode
{
    Token firstToken;
    std::map<Token, TokenGraphValue> values;
};

/**
 * @brief TokenGraph jest grafem gdzie kolejne miejsca w wektorze to połączone
 * listy tokenów. Na parzystych miejscach w polu firstToken znajdują się
 * oryginalne tokeny porównywanego pliku, zaś na nieparzystych miejscach
 * znajdują się tokeny występujące pomiędzy głównymi tokenami.
 */
typedef std::vector<TokenGraphNode> TokenGraph;

#endif // TOKENGRAPH_H
