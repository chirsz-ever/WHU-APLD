#ifndef GAMECONTEX_H
#define GAMECONTEX_H

#include <array>
#include <vector>
#include <ege.h>

using ChessPosition = std::array<int, 2>;

enum GameStatus
{
    CHOOSE_START_POINT,
    CHOOSE_OBSTRUCTIONS,
    BEFORE_DISPLAY_PATH,
    DISPLAY_PATH,
    UNFIND_PATH,
};

class GameContex
{
public:
    GameContex(int rows, int columns, int canvasWidth, int canvasHeight, int minPadding = 10);
    ~GameContex() = default;

    int rows()const {
        return m_rows;
    }
    int cols()const {
        return m_columns;
    }

    void drawChessBoard(ege::color_t lineColor, ege::PIMAGE dst = NULL)const;
    void drawChessPath(const std::vector<ChessPosition>& path, ege::color_t lineColor, bool delay = false, ege::PIMAGE dst = NULL)const;

    void input_mouse_down(int x, int y);

    std::vector<ChessPosition> search_path()const;

    GameStatus status;
    ChessPosition startPoint;
    std::vector<ChessPosition> obstructions;

private:
    int m_rows;
    int m_columns;
    int m_gridWidth;
    int m_xPadding;
    int m_yPadding;

    int chess_radius()const;
    ChessPosition position_nearby(int x, int y, int radius)const;

};

#endif // !GAMECONTEX_H
