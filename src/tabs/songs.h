#ifndef SONGS_TAB_H 
#define SONGS_TAB_H
#include "../tab.h"
#include "../panes/song_list.h"
namespace neemo { namespace tabs{
    class songs : public tab
    {
        public:
        songs(int row, int col);
        virtual void normal_mode(char c) override;
        virtual void edit_mode(char c) override;
        virtual void activate() override;

    };
}}
#endif
