#include "GameContex.h"
#include <algorithm>
#include <cmath>

int GameContex::chess_radius() const
{
    return m_gridWidth*0.38f;
}

GameContex::GameContex(int rows, int columns, int canvasWidth, int canvasHeight, int minPadding) :
    m_rows(rows), m_columns(columns)
{
    // 选择最大的格宽
    int g = (canvasHeight - 2 * minPadding) / (rows - 1);
    if (g*(columns - 1) + 2 * minPadding <= canvasWidth)
    {
        m_gridWidth = g;
    }
    else {
        g = (canvasWidth - 2 * minPadding) / (columns - 1);
        m_gridWidth = g;
    }

    m_yPadding = (canvasHeight - m_gridWidth*(rows - 1)) / 2;
    m_xPadding = (canvasWidth - m_gridWidth*(columns - 1)) / 2;
    status = CHOOSE_START_POINT;
    startPoint = { 0,0 };
}

void GameContex::drawChessBoard(ege::color_t lineColor, ege::PIMAGE dst)const
{
    int cx, cy;
    int cnt;
    int row_line_len = (m_columns - 1) * m_gridWidth;
    int col_line_len = (m_rows - 1) * m_gridWidth;

    ege::setcolor(lineColor, dst);
    // 画横线
    for (cy = m_yPadding, cnt = 0; cnt < m_rows; ++cnt)
    {
        ege::moveto(m_xPadding, cy, dst);
        ege::linerel(row_line_len, 0, dst);
        cy += m_gridWidth;
    }

    // 画竖线
    for (cx = m_xPadding, cnt = 0; cnt < m_columns; ++cnt)
    {
        ege::moveto(cx, m_yPadding, dst);
        ege::linerel(0, col_line_len, dst);
        cx += m_gridWidth;
    }

    const int ch_r = chess_radius();
    const int start_x = m_xPadding + startPoint[0] * m_gridWidth;
    const int start_y = m_yPadding + startPoint[1] * m_gridWidth;
    const int hcharw = ege::textwidth("馬", dst);
    const int hcharh = ege::textheight("馬", dst);
    // 画开始点
    ege::setcolor(ege::BLACK, dst);
    ege::setfillcolor(ege::RED, dst);
    ege::fillellipse(start_x, start_y, ch_r, ch_r, dst);
    if (hcharw*hcharw + hcharh*hcharh <= ch_r*ch_r * 4)
    {
        ege::setcolor(ege::WHITE, dst);
        ege::outtextxy(start_x - hcharw / 2, start_y - hcharh / 2, "馬", dst);
    }

    const int scharw = ege::textwidth("卒", dst);
    const int scharh = ege::textheight("卒", dst);
    const bool draw_char = scharw*scharw + scharh*scharh <= ch_r*ch_r * 4;
    // 画障碍点
    ege::setcolor(ege::WHITE, dst);
    ege::setfillcolor(ege::BLACK, dst);
    for (const ChessPosition& cp : obstructions)
    {
        const int ob_x = m_xPadding + cp[0] * m_gridWidth;
        const int ob_y = m_yPadding + cp[1] * m_gridWidth;
        ege::fillellipse(ob_x, ob_y, ch_r, ch_r, dst);
        if (draw_char)
            ege::outtextxy(ob_x - scharw / 2, ob_y - scharh / 2, "卒", dst);
    }
}

void GameContex::drawChessPath(const std::vector<ChessPosition>& path, ege::color_t lineColor, bool delay, ege::PIMAGE dst)const
{
    if (path.empty())
        return;

    ege::setcolor(lineColor, dst);
    ege::moveto(m_xPadding + path[0][0] * m_gridWidth, m_yPadding + path[0][1] * m_gridWidth, dst);
    for (const ChessPosition& cp : path)
    {
        ege::lineto(m_xPadding + cp[0] * m_gridWidth, m_yPadding + cp[1] * m_gridWidth, dst);
        if (delay)
        {
            ege::delay_ms(100 * 36.0f / (m_rows*m_columns));
        }
    }
}

ChessPosition GameContex::position_nearby(int x, int y, int radius) const
{
    int gx = lround(float(x) / m_gridWidth);
    int gy = lround(float(y) / m_gridWidth);
    if (gx >= 0 && gx < m_columns && gy >= 0 && gy < m_rows)
        if (abs(x - gx*m_gridWidth) < radius&&abs(y - gy*m_gridWidth) < radius)
        {
            return { gx,gy };
        }
    return { -1,-1 };
}

void GameContex::input_mouse_down(int x, int y)
{
    const int search_radius = m_gridWidth *0.3f;
    const int ch_r = chess_radius();
    const int rx = x - m_xPadding, ry = y - m_yPadding; // relative x,y
    ChessPosition cp;

    if ((cp = position_nearby(rx, ry, search_radius))[0] >= 0)
    {
        switch (status) {
        case CHOOSE_START_POINT:
            startPoint = cp;
            break;
        case CHOOSE_OBSTRUCTIONS: {
            auto it = std::find(obstructions.begin(), obstructions.end(), cp);
            if (it == obstructions.end())
            {
                obstructions.push_back(cp);
            }
            else
                obstructions.erase(it);
            break;
        }
        default:
            break;
        }
        return;
    }

    if ((cp = position_nearby(rx, ry, ch_r))[0] >= 0)
    {
        switch (status) {
        case CHOOSE_OBSTRUCTIONS: {
            auto it = std::find(obstructions.begin(), obstructions.end(), cp);
            if (it != obstructions.end())
                obstructions.erase(it);
            break;
        }
        default:
            break;
        }
    }
}
