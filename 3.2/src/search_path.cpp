#include "GameContex.h"
#include <algorithm>

using std::vector;

enum GridStatus
{
    NONE = 0,
    CHESS,
    FOOTPRINT,
};

class PathSearcher
{
public:
    PathSearcher(const GameContex& _gctx);
    ~PathSearcher() = default;

    const GameContex& gctx;

    GridStatus& chessboard(const ChessPosition& cp);

    int max_path_len();

    bool search_path();

    bool check_position(int x, int y);

    bool check_position(const ChessPosition& cp);

    std::vector<int> select_next_position();

    vector<ChessPosition> pathlog;

private:
    vector<GridStatus> m_chessboard;
    int k;
};

PathSearcher::PathSearcher(const GameContex& _gctx) :gctx(_gctx) {
    m_chessboard.resize(gctx.cols()*gctx.rows());
    pathlog.resize(max_path_len());
    pathlog[0] = gctx.startPoint;
    chessboard(pathlog[0]) = FOOTPRINT;
    for (const ChessPosition& ob : gctx.obstructions)
    {
        chessboard(ob) = CHESS;
    }
    k = 0;
}

inline int PathSearcher::max_path_len()
{
    return gctx.cols()*gctx.rows() - gctx.obstructions.size();
}

inline GridStatus & PathSearcher::chessboard(const ChessPosition& cp)
{
    return m_chessboard[cp[1]*gctx.cols() + cp[0]];
}

using Pace = std::array<int, 2>;

static inline ChessPosition move(const ChessPosition& pfrom, const Pace& pace)
{
    return { pfrom[0] + pace[0],pfrom[1] + pace[1] };
}

// 8���ƶ�����
const Pace hpaces[8] = { {1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2} };
//const int dx[8] = { 1,2,2,1,-1,-2,-2,-1 };
//const int dy[8] = { 2,1,-1,-2,-2,-1,1,2 };


// ���ƶ������Ӧ�ı����ӵ����λ��
const Pace blockades[8] = { { 0,1 },{ 1,0 },{ 1,0 },{ 0,-1 },{ 0,-1 },{ -1,0 },{ -1,0 },{ 0,1 } };


inline bool PathSearcher::check_position(int x, int y) {
    return 0 <= x && x < gctx.cols() && 0 <= y&&y < gctx.rows();
}

bool PathSearcher::check_position(const ChessPosition & cp)
{
    return check_position(cp[0], cp[1]);
}

std::vector<int> PathSearcher::select_next_position()
{
    vector<int> candidates;
    ChessPosition ccp = pathlog[k]; // current chess position
    int exits[8] = { 0 };
    for (int i = 0; i < 8; ++i)
    {
        ChessPosition cp = move(ccp, hpaces[i]);
        auto bcp = move(ccp, blockades[i]);
        if (check_position(cp) && chessboard(cp) == NONE && chessboard(bcp) != CHESS)
        {
            candidates.push_back(i);
        }
    }

    for (int n : candidates)
    {
        ChessPosition cp = move(ccp, hpaces[n]);
        for (int i = 0; i < 8; ++i)
        {
            ChessPosition ncp = move(cp, hpaces[i]);
            auto bncp = move(cp, blockades[i]);
            if (check_position(ncp) && chessboard(ncp) == NONE && chessboard(bncp) != CHESS)
            {
                ++exits[n];
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(), [&](const int&a, const int&b) {
        return exits[a] < exits[b];
    });

    return candidates;
}

// ����search_pathʱ��pathlog[k]�Ѿ����ã�Ҫ������λ��k+1
bool PathSearcher::search_path()
{
    // ����Ѿ��ɹ�����������·���㣬�򷵻�
    if (k == max_path_len() - 1)
    {
        return true;
    }

    // �������кϷ��Ŀ��Է���k+1λ���ϵ�·����
    // ����˳����select_next_position����
    auto candidates = select_next_position();
    for (int p : candidates)
    {
        pathlog[k + 1] = move(pathlog[k], hpaces[p]);
        ++k;
        chessboard(pathlog[k]) = FOOTPRINT;
        if (search_path())
            return true;
        chessboard(pathlog[k]) = NONE;
        --k;
    }
    return false;
}


std::vector<ChessPosition> GameContex::search_path() const
{
    PathSearcher psr(*this);

    if (psr.search_path())
        return psr.pathlog;
    else
        return {};
}
