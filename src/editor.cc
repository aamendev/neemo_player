#include "editor.h"
#include "common.h"
#include "src/tabs/queue.h"
#include "tabs/songs.h"
#include "tabs/bindings.h"
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>
#include <unistd.h>
namespace neemo 
{
    void editor::init()
    {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        g_editor_settings.width = w.ws_col;
        g_editor_settings.height = w.ws_row;
        tcgetattr(STDIN_FILENO, &_mold_terminal);
        _mnew_terminal = _mold_terminal;
        _mnew_terminal.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &_mnew_terminal);
        std::cout << "\033[2J\033[1;1H\033[?7l" << std::flush;
        m_tabs.push_back(new tabs::songs(DEFAULT_OFFSET_ROW, 5));
        m_tabs.push_back(new tabs::queue(DEFAULT_OFFSET_ROW, 5 + m_tabs[0]->m_name.size() + DEFAULT_OFFSET_COL));
        m_tabs.push_back(new tabs::bindings(DEFAULT_OFFSET_ROW, 
                    5 + m_tabs[1]->m_name.size() + m_tabs[0]->m_name.size() + 2 * DEFAULT_OFFSET_COL));
        render_tabs();
    }
    void editor::render_init()
    {
        set_cursor(0 , 0, 0, 0);
        std::cout << "\x1b[0J" << std::flush;
        render_tabs();
    }
    void editor::render_tabs()
    {
        auto sz = 0;
        for (auto t : m_tabs)
        {
            sz += (t->m_name.size() + DEFAULT_OFFSET_COL);
        }
        int col = (g_editor_settings.width - sz)/2;
        int row = 5;
        for (int i = 0 ; i < m_tabs.size(); i++)
        {
            auto e = m_tabs[i];
            set_cursor(row, col, 0, 0);
            if (i == m_curr_tab)
            {
                write_with_fg_bg(e->m_name,
                        selected_fg.r, selected_fg.g, selected_fg.b,
                        selected_bg.r, selected_bg.g, selected_bg.b
                        );
            }
            else 
            {
                std::cout << e->m_name;
            }
            col += e->m_name.size() + DEFAULT_OFFSET_COL;
        }
        set_cursor(row + 1, 0, 0, 0);
        std::cout << "\x1b[0J" << std::flush;
        m_tabs[m_curr_tab]->activate();
    }
    void editor::render_tab(int idx)
    {
        auto sz = 0;
        for (auto t : m_tabs)
        {
            sz += (t->m_name.size() + DEFAULT_OFFSET_COL);
        }
        int col = (g_editor_settings.width - sz)/2;
        int row = 5;
        for (int i = 0 ; i < idx; i++)
        {
            auto e = m_tabs[i];
            col += e->m_name.size() + DEFAULT_OFFSET_COL;
        }
        set_cursor(row, col, 0, 0);
        auto e = m_tabs[idx];
        std::cout << e->m_name << std::flush;
    }
    void editor::render_selected()
    {
        auto sz = 0;
        for (auto t : m_tabs)
        {
            sz += (t->m_name.size() + DEFAULT_OFFSET_COL);
        }
        int col = (g_editor_settings.width - sz)/2;
        int row = 5;
        for (int i = 0 ; i < m_curr_tab; i++)
        {
            auto e = m_tabs[i];
            col += e->m_name.size() + DEFAULT_OFFSET_COL;
        }
        set_cursor(row, col, 0, 0);
        auto e = m_tabs[m_curr_tab];
        write_with_fg_bg(e->m_name,
                selected_fg.r, selected_fg.g, selected_fg.b,
                selected_bg.r, selected_bg.g, selected_bg.b
                );
        set_cursor(row + 1, 0, 0, 0);
        std::cout << "\x1b[0J" << std::flush;
    }
    void editor::run()
    {
        while (!g_editor_settings.should_close)
        {
            m_timer.begin();
            char c = get_char();
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            if (w.ws_col != g_editor_settings.width || w.ws_row != g_editor_settings.height)
            {
                g_editor_settings.width = w.ws_col;
                g_editor_settings.height = w.ws_row;
                render_init();
            }
            set_cursor( DEFAULT_OFFSET_ROW - 1, 2 * g_editor_settings.width/3, 0, 0);
            std::cout << c << std::flush;
            switch (g_editor_settings.mode)
            {
                case command_mode::normal:
                    {
                        switch (c)
                        {
                            case '1':
                            case '2':
                            case '3':
                            case '4':
                            case '5':
                            case '6':
                            case '7':
                            case '8':
                            case '9':
                                {
                                    render_tab(m_curr_tab);
                                    m_curr_tab = std::clamp(c-'1', 0, (int)m_tabs.size() - 1);
                                    render_selected();
                                    m_tabs[m_curr_tab]->activate();
                                }
                                break;
                            case 'q':
                                {
                                    g_editor_settings.should_close = true;
                                }
                                break;
                            default:{m_tabs[m_curr_tab]->normal_mode(c);}
                        }
                    }
                case neemo::command_mode::edit:
                    {
                        m_tabs[m_curr_tab]->edit_mode(c);
                    }
            }
            m_tabs[m_curr_tab]->tick();
            m_timer.end();
            m_timer.wait(1000./60 - m_timer.get_time());
        }
    }
    void editor::terminate()
    {

        for (auto& t : m_tabs)
        {
            for (auto& p : t->m_panes)
            {
                delete p;
            }
            t->m_panes.clear();
            delete t;
        }
        m_tabs.clear();
        set_cursor(0 , 0, 0, 0);
        std::cout << "\x1b[0J\033[?7h" << std::flush;
        tcsetattr(STDIN_FILENO, TCSANOW, &_mold_terminal);
    }
    char editor::get_char() {
        char buf = 0;
        read(STDIN_FILENO, &buf, 1);
        return buf;
    }
}
