#include "graph.h"
#include "load_gph.h"
#include <string>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <fstream>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

using Graph = chirsz::Graph<string>;
using VertexID = Graph::VertexID;

enum EdgeType
{
    ToLeft,
    ToRight,
    Bidirect,
};

class Lexer
{
public:
    Lexer(const string&s): sentence(s) {}
    string nextWord();
    EdgeType nextEdge();
    bool touchEnd();

private:
    const string& sentence;
    size_t k = 0;
};

std::unique_ptr<Graph> chirsz::load_gph(const char* filename)
{
    chirsz::GraphBuilder<string> GB;
    std::unordered_map<string, VertexID> vid_dict;
    std::ifstream fin(filename);
    if (!fin)
    {
        cout << "open " << filename << " failed" << endl;
        exit(0);
    }
    for (string line; getline(fin, line); )
    {
        auto lexer = Lexer(line);

        if (lexer.touchEnd())
            continue;

        string last_word = lexer.nextWord();
        if (vid_dict.find(last_word) == vid_dict.end())
            vid_dict[last_word] = GB.add_vertex(last_word);
        while (!lexer.touchEnd())
        {
            EdgeType et = lexer.nextEdge();
            string word = lexer.nextWord();
            if (vid_dict.find(word) == vid_dict.end())
                vid_dict[word] = GB.add_vertex(word);
            switch (et)
            {
            case ToLeft:
                GB.add_edge(vid_dict[word], vid_dict[last_word]);
                break;
            case ToRight:
                GB.add_edge(vid_dict[last_word], vid_dict[word]);
                break;
            case Bidirect:
                GB.add_edge(vid_dict[word], vid_dict[last_word]);
                GB.add_edge(vid_dict[last_word], vid_dict[word]);
                break;
            }
            last_word = word;
        }
    }
    return GB.genarate();

}


string Lexer::nextWord()
{
    if (touchEnd())
    {
        throw std::runtime_error("End on where except a word");
    }
    while (isspace(sentence[k]))++k;
    string word;
    while (k < sentence.size() && isalnum(sentence[k]))
    {
        word.push_back(sentence[k]);
        ++k;
    }
    if (word.empty())
    {
        throw std::runtime_error("expectd a valid edge");
    }
    return word;
}

EdgeType Lexer::nextEdge()
{
    while (isspace(sentence[k]))++k;
    cout.flush();
    if (k > sentence.size() - 2)
    {
        throw std::runtime_error("End on where except a edge");
    }
    string pat = sentence.substr(k, 2);
    if (pat == "->")
    {
        k += 2;
        return ToRight;
    }
    else if (pat == "<-")
    {
        k += 2;
        return ToLeft;
    }
    else if (k <= sentence.size() - 3 && sentence.substr(k, 3) == "<=>")
    {
        k += 3;
        return Bidirect;
    }
    else
    {
        throw std::runtime_error("could not read a vaild edge");
    }

}

bool Lexer::touchEnd()
{
    size_t k1 = k;
    while (isspace(sentence[k1]) && k1 < sentence.size())++k1;
    return k1 == sentence.size();
}
