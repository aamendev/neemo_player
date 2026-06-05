#include "queue.h"
#include "src/common.h"
#include "src/panes/queue_list.h"
#include "src/panes/song_list.h"
namespace neemo { namespace tabs{
    queue::queue(int row, int col)
    {
        m_name = "queue";
        m_panes.push_back(new panes::queue_list(row, col));
        init();
    }
    void queue::tick()
    {
        for (auto p : m_panes)
        {
            p->tick();
        }
    }
    void queue::activate()
    {
        m_panes[m_curr_active_pane]->render();
    }
    void queue::normal_mode(char c)
    {
        switch(c)
        {
            case 'l':
            {
                m_curr_active_pane += 1;
                m_curr_active_pane %= m_panes.size();
            }
            break;
            case 'h':
            {
                m_curr_active_pane = 
                    (m_curr_active_pane - 1) * (m_curr_active_pane > 0) + 
                    (m_panes.size() - 1) * !(m_curr_active_pane > 0);
            }
            break;
            default: {m_panes[m_curr_active_pane]->normal_mode(c);}
        }
    }
    void queue::edit_mode(char c)
    {
        m_panes[m_curr_active_pane]->edit_mode(c);
    }
}}
