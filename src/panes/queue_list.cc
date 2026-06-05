#include "queue_list.h"
#include "src/common.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#define MAX_SONG_LENGTH (2048)
namespace neemo { namespace panes {
    queue_list::queue_list(int parent_row, int parent_col)
    {
        m_parent_row = parent_row + 1;
        m_parent_col = 0;
        tick();
    }
    queue_list::~queue_list()
    {
        m_elements.clear();
        run_system("mpc clear");
    }
    void queue_list::tick()
    {
        bool updated = !g_editor_settings.song_queue.empty();
        while (!g_editor_settings.song_queue.empty())
        {
            m_elements.push_back(g_editor_settings.song_queue.front());
            auto str = "mpc add \"" + g_editor_settings.song_queue.front() + "\"";
            run_system(str.c_str());
            g_editor_settings.song_queue.pop();
        }
        if (updated)
        {
            render();
            auto str = get_mpc_state();
            if (str == "stopped")
            {
                run_system("mpc play");
            }
        }
    }
    void queue_list::render()
    {
        int row = DEFAULT_OFFSET_ROW;
        int col = DEFAULT_OFFSET_COL;
        set_cursor(row, col, m_parent_row, m_parent_col);
        std::cout << "\x1b[0J" << std::flush;
        for (int i = m_min_scroll ; i < std::min(m_min_scroll + g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row + 1, (int)m_elements.size()); i++)
        {
            auto e = m_elements[i];
            set_cursor(row, col, m_parent_row, m_parent_col);
            if (i == m_curr_active_element)
            {
                write_with_fg_bg(e,
                        selected_fg.r, selected_fg.g, selected_fg.b,
                        selected_bg.r, selected_bg.g, selected_bg.b
                        );
            }
            else 
            {
                std::cout << e;
            }
            row += 1;
        }
        set_cursor(DEFAULT_OFFSET_ROW + m_curr_active_element - m_min_scroll, col, m_parent_row, m_parent_col);
        std::cout << std::flush;
    }
    void queue_list::render_selected()
    {
        int row = DEFAULT_OFFSET_ROW + m_curr_active_element - m_min_scroll;
        int col = DEFAULT_OFFSET_COL;
        set_cursor(row, col, m_parent_row, m_parent_col);
        auto e = m_elements[m_curr_active_element];
        write_with_fg_bg(e,
                selected_fg.r, selected_fg.g, selected_fg.b,
                selected_bg.r, selected_bg.g, selected_bg.b
                );
        set_cursor(row, col, m_parent_row, m_parent_col);
        std::cout << std::flush;
    }
    void queue_list::render_element(int i)
    {
        int row = DEFAULT_OFFSET_ROW + i - m_min_scroll;
        int col = DEFAULT_OFFSET_COL;
        set_cursor(row, col, m_parent_row, m_parent_col);
        auto e = m_elements[m_curr_active_element];
        std::cout << e << std::flush;
    }

    void queue_list::normal_mode(char c)
    {
        if (m_elements.empty()) return;
        switch (c)
        {
            case 'g':
                {
                    render_element(m_curr_active_element);
                    m_curr_active_element = 0;
                    if (m_curr_active_element < m_min_scroll)
                    {
                        m_min_scroll = 0;
                        render();
                    }
                    else 
                    {
                        render_selected();
                    }
                }
                break;
            case 'b':
                {
                    render_element(m_curr_active_element);
                    m_curr_active_element = std::max(0, (int)m_elements.size() - 1);
                    if (m_curr_active_element > m_min_scroll + (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row))
                    {
                        m_min_scroll = std::max(0, (int)m_elements.size() - (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row));
                        render();
                    }
                    else 
                    {
                        render_selected();
                    }
                }
                break;
            case 'j':
                {
                    render_element(m_curr_active_element);
                    m_curr_active_element = std::max(0, std::min(m_curr_active_element + 1, (int)m_elements.size() - 1));
                    if (m_curr_active_element > m_min_scroll + (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row))
                    {
                        m_min_scroll++;
                        render();
                    }
                    else 
                    {
                        render_selected();
                    }
                }
                break;
            case 'k':
                {
                    render_element(m_curr_active_element);
                    m_curr_active_element = std::max(m_curr_active_element - 1, 0);
                    if (m_curr_active_element < m_min_scroll)
                    {
                        m_min_scroll = std::max(m_min_scroll - 1, 0);
                        render();
                    }
                    else 
                    {
                        render_selected();
                    }
                }
                break;

            case 'm':
                {
                    render_element(m_curr_active_element);
                    auto e = m_elements[m_curr_active_element];
                    auto pos = e.find_last_of('/');
                    if (pos != std::string::npos)
                    {
                        int skip = 0;
                        for (int i = 0; i < m_elements.size(); i++)
                        {
                            auto el = m_elements[i];
                            if (std::includes(
                                        el.begin(), el.end(), 
                                        e.begin(), e.begin() + pos + 1
                                        ))
                            {
                                skip = i;
                            }
                        }
                        m_curr_active_element = std::min(skip + 1, (int)m_elements.size() - 1);
                        if (m_curr_active_element > m_min_scroll + (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row))
                        {
                            m_min_scroll = std::max(0,
                                    std::min(
                                        (int)m_elements.size() - (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row), 
                                        skip+1));
                            render();
                        }
                        else 
                        {
                            render_selected();
                        }
                    }
                    else 
                    {
                        m_curr_active_element = std::min(m_curr_active_element + 1, (int)m_elements.size() - 1);
                        if (m_curr_active_element > m_min_scroll + (g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row))
                        {
                            m_min_scroll++;
                            render();
                        }
                        else 
                        {
                            render_selected();
                        }
                    }
                }
                break;
            case ',':
                {
                    render_element(m_curr_active_element);
                    auto e = m_elements[m_curr_active_element];
                    auto pos = e.find_last_of('/');
                    if (pos != std::string::npos)
                    {
                        int skip = 0;
                        for (int i = m_elements.size() - 1; i > -1; i--)
                        {
                            auto el = m_elements[i];
                            if (std::includes(
                                        el.begin(), el.end(), 
                                        e.begin(), e.begin() + pos + 1
                                        ))
                            {
                                skip = i;
                            }
                        }
                        m_curr_active_element = std::max(skip - 1, 0);
                        if (m_curr_active_element < m_min_scroll)
                        {
                            m_min_scroll = m_curr_active_element;
                            render();
                        }
                        else 
                        {
                            render_selected();
                        }
                    }
                    else 
                    {
                        m_curr_active_element = std::max(m_curr_active_element - 1, 0);
                        if (m_curr_active_element < m_min_scroll)
                        {
                            m_min_scroll = std::max(m_min_scroll - 1, 0);
                            render();
                        }
                        else 
                        {
                            render_selected();
                        }

                    }
                }
                break;
            case 'p':
                {
                    run_system("mpc toggle");
                }
                break;
            case 'd':
                {
                    auto str = "mpc del " + std::to_string(m_curr_active_element + 1);
                    run_system(str.c_str());
                    m_elements.erase(m_elements.begin() + m_curr_active_element);
                    m_curr_active_element = std::max(0, std::min(m_curr_active_element, (int)m_elements.size() - 1));
                    render();
                }
                break;
            case '\n':
                {
                    auto str = "mpc play " + std::to_string(m_curr_active_element + 1);
                    run_system(str.c_str());
                }
                break;
            case ';':
                {
                    auto e = m_elements[m_curr_active_element];
                    auto pos = e.find_last_of('/');
                    if (pos != std::string::npos)
                    {
                        std::vector<int> to_erase;
                        for (int i = m_elements.size() - 1; i > -1; i--)
                        {
                            auto el = m_elements[i];
                            if (std::includes(
                                        el.begin(), el.end(), 
                                        e.begin(), e.begin() + pos + 1
                                        ))
                            {
                                auto str = "mpc del " + std::to_string(i + 1);
                                run_system(str.c_str());
                                to_erase.push_back(i);
                            }
                        }
                        for (auto i : to_erase)
                        {
                            m_elements.erase(m_elements.begin() + i);
                        }
                    }
                    else 
                    {
                        auto str = "mpc del " + std::to_string(m_curr_active_element + 1);
                        run_system(str.c_str());
                        m_elements.erase(m_elements.begin() + m_curr_active_element);
                    }
                    m_curr_active_element = std::max(0, std::min(m_curr_active_element, (int)m_elements.size() - 1));
                    render();
                }
                break;
        }
    }
    void queue_list::edit_mode(char c)
    {
    }
    std::string queue_list::get_mpc_state()
    {
        FILE* pipe = popen("mpc status %state%", "r");
        if (!pipe) 
        {
            return "error";
        }

        char buffer[128];
        std::string result;

        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) 
        {
            result += buffer;
        }

        pclose(pipe);

        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    }
}}
