#ifndef TOKEN_H
#define TOKEN_H

#define DEF_TOKEN_OP(x) \
    bool operator x (const Token& token) const \
    { return this->id x token.id; }

/**
 * @brief Struktura tokenu. Zawiera dodatkowe informacje pozwalającą na szybkie
 * porównywanie różnych tokenów. Numer id jest generowany przez pomocniczą
 * klasę FileHelper podczas etapu tokenizowania.
 */
struct Token
{
    enum TokenType
    {
        NUMBER,
        STRING,
        KEYWORD,
        SIGN,
        NEW_LINE,
        TOKEN_NUM,
    };

    Token() : type(NUMBER), value(), id(0) {}

    DEF_TOKEN_OP(==)
    DEF_TOKEN_OP(!=)
    DEF_TOKEN_OP(<)
    DEF_TOKEN_OP(<=)
    DEF_TOKEN_OP(>)
    DEF_TOKEN_OP(>=)

    TokenType type;
    std::string value;
    long id;
};

#endif // TOKEN_H
