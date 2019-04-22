
#include "ege.h"
#include "microui_impl_ege.h"
#include "microui.h"
#include "GameContex.h"
#include <cstdio>
#include <ctime>

const char* labeltexts[] = { u8"请选择一个开始点",
                             u8"请选择零个或几个障碍点",
                             u8"点击\"下一步\"按钮以开始\n寻找可行路径",
                             u8"找到一条可行路径",
                             u8"未找到可行路径"
                           };

static void full_window(mu_Context *ctx, GameStatus status, bool* btn_pressed) {
    static mu_Container window;

    /* init window manually so we can set its position and size */
    if (!window.inited) {
        mu_init_window(ctx, &window, MU_OPT_NOINTERACT);
        window.rect = mu_rect(0, 0, 800, 600);
    }
    if (mu_begin_window_ex(ctx, &window, "", MU_OPT_NOTITLE | MU_OPT_NOINTERACT | MU_OPT_NORESIZE)) {
        mu_layout_set_next(ctx, mu_rect(600, 150, 100, 100), 0);
        mu_text(ctx, labeltexts[status]);
        mu_layout_set_next(ctx, mu_rect(650, 280, 100, 40), 0);
        if (status != DISPLAY_PATH&&status != UNFIND_PATH) {
            if (mu_button(ctx, u8"下一步")) *btn_pressed = true;
        }
        mu_end_window(ctx);
    }
}

static void getargs(int argc, const char *argv[], int& rows, int& cols) {
    switch (argc)
    {
    case 1:
        rows = cols = 8;
        break;
    case 3: {
        std::string buf = std::string(argv[1]) + " " + argv[2];
        sscanf(buf.c_str(), "%d %d", &rows, &cols);
        if ((rows > 4 || cols > 4) && rows > 2 && cols > 2)
            break;
    }
    default:
        MessageBox(nullptr, "输入的行、列数都要大于2且至少一个大于4！", "错误的命令行参数", MB_OK);
        exit(1);
    }
}


int main(int argc, const char *argv[]) {
    int rows, cols;
    getargs(argc, argv, rows, cols);
    GameContex gctx(rows, cols, 600, 600, 50);
    std::vector<ChessPosition> path;

    r_init(800, 600, ege::INIT_ANIMATION&~ege::INIT_WITHLOGO);
    ::SetWindowText(ege::getHWnd(), "Horse");

    /* init microui */
    mu_Context ctx_c;
    mu_Context *ctx = &ctx_c;
    mu_init(ctx);
    ctx->text_width = r_get_text_width;
    ctx->text_height = r_get_text_height;
    ctx->style->colors[MU_COLOR_WINDOWBG] = mu_color(255, 255, 255, 255);
    ctx->style->colors[MU_COLOR_BUTTON] = mu_color(128, 128, 128, 255);
    ctx->style->colors[MU_COLOR_BUTTONHOVER] = mu_color(230, 230, 230, 255);
    ctx->style->colors[MU_COLOR_BUTTONFOCUS] = mu_color(200, 200, 200, 255);
    ctx->style->colors[MU_COLOR_TEXT] = mu_color(0, 0, 0, 255);


    /* main loop */
    while (ege::is_run()) {

        bool btn_pressed = false;
        GameStatus next_status = gctx.status;

        /* handle events */
        static bool use_input = false;
        if (use_input) {
            ege::mouse_msg mmsg;
            clock_t tic = clock();
            do {
                mmsg = ege::getmouse();
                ege2mu_input_mouse(ctx, mmsg);
            } while (mmsg.is_move() && (clock() - tic) < CLOCKS_PER_SEC / 60);

            if (mmsg.is_down())
                gctx.input_mouse_down(mmsg.x, mmsg.y);
        }
        else {
            use_input = true;
        }

        /* process frame */
        mu_begin(ctx);
        full_window(ctx, gctx.status, &btn_pressed);
        mu_end(ctx);

        /* process Game */
        switch (gctx.status) {
        case CHOOSE_START_POINT:
            if (btn_pressed) {
                next_status = CHOOSE_OBSTRUCTIONS;
            }
            break;
        case CHOOSE_OBSTRUCTIONS:
            if (btn_pressed) {
                next_status = BEFORE_DISPLAY_PATH;
            }
            break;
        case BEFORE_DISPLAY_PATH:
            if (btn_pressed) {
                path = gctx.search_path();
                next_status = path.size() ? DISPLAY_PATH : UNFIND_PATH;
            }
            break;
        default: // DISPLAY_PATH, UNFIND_PATH
            break;
        }

        /* render */
        mu_Command *cmd = NULL;
        while (mu_next_command(ctx, &cmd)) {
            switch (cmd->type) {
            case MU_COMMAND_TEXT:
                r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color);
                break;
            case MU_COMMAND_RECT:
                r_draw_rect(cmd->rect.rect, cmd->rect.color);
                break;
            case MU_COMMAND_ICON:
                r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color);
                break;
            case MU_COMMAND_CLIP:
                r_set_clip_rect(cmd->clip.rect);
                break;
            }
        }

        gctx.drawChessBoard(EGERGB(140, 140, 255));
        if (gctx.status == DISPLAY_PATH) {
            gctx.drawChessPath(path, ege::RED, true);
            break;
        }
        else if (gctx.status == UNFIND_PATH)
        {
            break;
        }

        gctx.status = next_status;
        r_flush();
    }

    while (ege::is_run()) ege::delay_fps(24);

    r_end();

    return 0;
}

