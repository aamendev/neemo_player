#ifndef QUEUE_LIST_H
#define QUEUE_LIST_H
#include "../pane.h"
#include "src/common.h"
namespace neemo { namespace panes { 
    class queue_list : public pane 
    {
        public:
            queue_list(int parent_row, int parent_col);
            virtual ~queue_list();
            virtual void normal_mode(char c) override;
            virtual void edit_mode(char c) override;
            virtual void render() override;
            virtual void tick() override;
            int m_min_scroll = 0;
        private:
            void render_selected();
            void render_element(int i);

            // written by chatgpt
            std::string get_mpc_state();
    };
}}
#endif
