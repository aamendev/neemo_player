#ifndef SONG_LIST_PANE_H
#define SONG_LIST_PANE_H
#include "../pane.h"
#include "src/common.h"
namespace neemo { namespace panes { 
    class song_list : public pane 
    {
        public:
            song_list(int parent_row, int parent_col);
            virtual void normal_mode(char c) override;
            virtual void edit_mode(char c) override;
            virtual void render() override;
            int m_min_scroll = 0;
        private:
            void load_songs();
            void render_selected();
            void render_element(int i);
    };
}}
#endif
