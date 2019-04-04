#include "graph.h"
#include "load_gph.h"
#include <string>
#include <stack>
#include <memory>
#include <fstream>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::unordered_map;

using Graph = chirsz::Graph<string>;
using VertexID = Graph::VertexID;

static string get_args(int argc, char const *argv[]);
static void visit(const unique_ptr<Graph> &G, VertexID v);
static void poll(const unique_ptr<Graph> &G, unordered_map<VertexID, bool>& visited);

int main(int argc, char const *argv[])
{
    string gph_file_name = get_args(argc, argv);
    auto G = chirsz::load_gph(gph_file_name.c_str());
    auto vertices = G->vertices();

    // 初始化访问标志
    unordered_map<VertexID, bool> visited;
    for (auto v : vertices)
        visited[v] = false;

    // 储存遍历路径的栈
    std::stack<VertexID> stk;

    // DFS
    for (auto v : vertices)
        if (!visited[v])
        {
            stk.push(v);
            while (!stk.empty())
            {
                // 出栈一个节点u并访问之
                VertexID u = stk.top();
                stk.pop();
                if (visited[u])
                    continue;

                visit(G, u);
                visited[u] = true;
                //poll(G, visited);

                // 让u的所有未访问过的邻居入栈
                for (auto w : G->adjacent_vertices(u))
                    if (!visited[w])
                        stk.push(w);
            }
            cout << endl;
        }

    return 0;
}

static string get_args(int argc, char const *argv[])
{
    string fname;
    switch (argc)
    {
    case 1:
        fname = "graph.gph";
        break;
    case 2:
        fname = argv[1];
        break;
    default:
        cout << "error arguments" << endl;
        exit(1);
    }
    return fname;
}

static void visit(const unique_ptr<Graph> &G, VertexID v)
{
    cout << "visit " << G->get_vdata(v) << endl;
}

static void poll(const unique_ptr<Graph> &G, unordered_map<VertexID, bool>& visited)
{
    for (auto vid : G->vertices())
    {
        printf("%s\t", G->get_vdata(vid).c_str());
    }
    cout << endl;
    for (auto vid : G->vertices())
    {
        const char* b = visited[vid] ? "true" : "false";
        printf("%s\t", b);
    }
    cout << endl;
}
