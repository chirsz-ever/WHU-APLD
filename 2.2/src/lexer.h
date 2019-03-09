#ifndef LEX_PARSER_H
#define LEX_PARSER_H

#include <string>
#include <token.h>

namespace chirsz {

class Lexer
{
public:
    Lexer(std::string);

    // 读取一个标记，并返回这个标记的类型。
    // 如果标记附带值（如Num类型），则可通过tokenValue()取得对应值
    // 如果全部解析完毕，返回None
    TokenType next();

    // 向前看一个标记，影响last_value()
    TokenType peek();

    // 消费一个标记，影响last_value()
    void consume();

    // 返回最近一次读取标记附带的值，
    // 只在最近一次读取的标记类型确实附带值时保证有效
    TokenValue last_value()const;

    // 返回当前词法解析器在句子上的位置
    size_t position()const;

private:
    std::string m_sentence;
    TokenValue m_last_value;
    size_t m_position;
    size_t m_last_token_len;
};

}

#endif
