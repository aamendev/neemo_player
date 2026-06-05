#ifndef BINDINGS_H
#define BINDINGS_H
#include "../tab.h"
#include "../panes/bindings_list.h"
namespace neemo { namespace tabs{
    class bindings : public tab
    {
        public:
        bindings(int row, int col) 
        {
            m_name="bindings"; 
            m_panes.push_back(new panes::bindings_list(row, col)); 
            init();
        }
        inline virtual void normal_mode(char c)override{};
        inline virtual void edit_mode(char c)override{};
        inline virtual void activate() override {m_panes[m_curr_active_pane]->render();};
    };
}}
#endif
