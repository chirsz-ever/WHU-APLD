#include <lexer.h>
#include <string>
#include <cctype>
#include <cstdio>
#include <stdexcept>

using std::string;
using std::to_string;

namespace chirsz {

Lexer::Lexer(string s): m_sentence(s), m_position(0), m_last_token_len(0) {}


TokenType Lexer::next() {

    // 忽略空白字符
    while (m_position < m_sentence.size()) {
        if (!isspace(m_sentence[m_position])) {
            break;
        }
        ++m_position;
    }

    if (m_position >= m_sentence.size())
    {
        m_last_token_len = 0;
        return TokenType::None;
    }

    char token = m_sentence[m_position];

    if (isdigit(token)) {
        int cnt;
        double num_val;
        sscanf(m_sentence.c_str() + m_position, "%lf%n", &num_val, &cnt);
        m_position += cnt;
        m_last_token_len = cnt;
        m_last_value.num = num_val;
        return TokenType::Num;
    }
    else if (islower(token) || isupper(token)) {
        m_position += 1;
        m_last_value.symbol = token;
        m_last_token_len = 1;
        return TokenType::Symbol;
    }
    else {
        m_last_token_len = 1;
        switch (token) {
        case '(': ++m_position; return TokenType::LPt;
        case ')': ++m_position; return TokenType::RPt;
        case '+': ++m_position; return TokenType::Add;
        case '-': ++m_position; return TokenType::Sub;
        case '*': ++m_position; return TokenType::Mul;
        case '/':
            if (m_sentence[m_position + 1] == '/')
            {
                m_position += 2;
                return TokenType::ExactDiv;
            }
            else {
                ++m_position;
                return TokenType::Div;
            }
        case '%': ++m_position; return TokenType::Mod;
        case '^': ++m_position; return TokenType::Pow;
        default:
            throw std::runtime_error(
                string("unknown character '") + token + "' on " +
                to_string(m_position + 1));
        }
    }
}


// 向前看一个标记
TokenType Lexer::peek() {
    const auto l_position = m_position;
    const auto ll_token_len = m_last_token_len;
    auto tt = next();
    m_position = l_position;
    m_last_token_len = ll_token_len;
    return tt;
}

// 消费一个标记
void Lexer::consume() {
    next();
}

// 返回最近一次读取标记附带的值，
// 只在最近一次读取的标记类型确实附带值时保证有效
TokenValue Lexer::last_value()const {
    return m_last_value;
}

// 返回当前词法解析器在句子上的位置
size_t Lexer::position()const {
    return m_position - m_last_token_len ;
}

}
