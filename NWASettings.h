#ifndef NWASETTINGS_H
#define NWASETTINGS_H
#include <array>
#include "Token.h"

struct NWASettings
{
    std::array<std::array<int, Token::TOKEN_NUM>, Token::TOKEN_NUM> equalMatrix;
    std::array<int, Token::TOKEN_NUM> unequalVector;
    int basePenalty;
    int penaltyReduction;
    int minPenalty;
};

#endif // NWASETTINGS_H
