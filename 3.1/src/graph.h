#ifndef GRAPH_H
#define GRAPH_H

#include <cstdint>
#include <vector>
#include <list>
#include <unordered_map>
#include <memory>
#include <algorithm>

namespace chirsz {

template <typename _Data>
class GraphBuilder;

template <typename _Data>
class Graph {
public:
    typedef uint64_t VertexID;
    typedef _Data VDataType;

    std::vector<VertexID> vertices()const;

    std::vector<VertexID> adjacent_vertices(VertexID _vid)const;

    bool is_edge(VertexID _vfrom, VertexID _vto)const;

    const VDataType& get_vdata(VertexID _vid)const;

private:
    Graph() = default;
    std::vector<VDataType> m_v_data;
    std::vector<std::list<VertexID>> m_adjlists;
    friend class GraphBuilder<_Data>;
};

template <typename _Data>
class GraphBuilder
{
public:
    typedef typename Graph<_Data>::VertexID VertexID;
    typedef typename Graph<_Data>::VDataType VDataType;

    GraphBuilder() = default;

    VertexID add_vertex(const VDataType& _vdata);

    void add_edge(VertexID _vfrom, VertexID _vto);

    std::unique_ptr<Graph<_Data>> genarate()const;

private:
    std::unordered_map<VertexID, VDataType> m_v_data;
    std::unordered_map<VertexID, std::list<VertexID>> m_v_adj;
};

template <typename _Data>
std::vector<typename Graph<_Data>::VertexID> Graph<_Data>::vertices()const
{
    std::vector<VertexID> vs(m_v_data.size());
    for (size_t i = 0; i < m_v_data.size(); ++i)
        vs[i] = i;
    return vs;
}

template <typename _Data>
std::vector<typename Graph<_Data>::VertexID> Graph<_Data>::adjacent_vertices(VertexID _vid)const
{
    const std::list<VertexID>& adjl = m_adjlists[_vid];
    return std::vector<VertexID>(adjl.begin(), adjl.end());
}

template <typename _Data>
bool Graph<_Data>::is_edge(VertexID _vfrom, VertexID _vto)const
{
    std::list<VertexID>& adjl = m_adjlists[_vfrom];
    return std::find(adjl.begin(), adjl.end(), _vto) == adjl.end();
}

template <typename _Data>
const _Data& Graph<_Data>::get_vdata(VertexID _vid)const
{
    return m_v_data[_vid];
}

template <typename _Data>
typename GraphBuilder<_Data>::VertexID GraphBuilder<_Data>::add_vertex(const VDataType& _vdata)
{
    size_t n = m_v_data.size();
    m_v_data[n] = _vdata;
    return n;
}

template <typename _Data>
void GraphBuilder<_Data>::add_edge(VertexID _vfrom, VertexID _vto)
{
    m_v_adj[_vfrom].push_back(_vto);
}

template <typename _Data>
std::unique_ptr<Graph<_Data>> GraphBuilder<_Data>::genarate()const
{
    auto G = std::unique_ptr<Graph<_Data>>(new Graph<_Data>);
    G->m_v_data.resize(m_v_data.size());
    G->m_adjlists.resize(m_v_adj.size());
    for (size_t i = 0; i < m_v_data.size(); ++i)
    {
        G->m_v_data[i] = m_v_data.at(i);
    }
    for (size_t i = 0; i < m_v_adj.size(); ++i)
    {
        G->m_adjlists[i] = m_v_adj.at(i);
    }
    return G;
}

}

#endif
