#include "bindings_list.h"
namespace neemo { namespace panes { 
    bindings_list::bindings_list(int parent_row, int parent_col)
    {
        m_parent_row = parent_row + 1;
        m_parent_col = 0;
        m_elements.push_back("j, k \tup/down one element within window");
        m_elements.push_back("h, l \tleft/right one element within tab if with multiple windows");
        m_elements.push_back("m/,  \tup/down directory (skip all elements in the same deepest subdirectory)");
        m_elements.push_back("g/b  \tgo to top/bottom of page");
        m_elements.push_back("enter\tadd element to queue / play element in queue");
        m_elements.push_back(";    \tadd/remove deepest diretory to queue (based on current tab)");
        m_elements.push_back("d    \tdelete current element from queue");
        m_elements.push_back("1-9  \tload tab n");
    }
    void bindings_list::render()
    {
        int row = DEFAULT_OFFSET_ROW;
        int col = DEFAULT_OFFSET_COL;
        set_cursor(row, col, m_parent_row, m_parent_col);
        std::cout << "\x1b[0J" << std::flush;
        for (int i = 0 ; i < std::min(g_editor_settings.height - DEFAULT_OFFSET_ROW - m_parent_row + 1, (int)m_elements.size()); i++)
        {
            auto e = m_elements[i];
            set_cursor(row, col, m_parent_row, m_parent_col);
            std::cout << e;
            row += 1;
        }
        std::cout << "\33[?25l";
        std::cout << std::flush;
        std::cout << "\33[?25h";
    }
}}
