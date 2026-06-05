#ifndef EDITOR_H
#define EDITOR_H
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include "common.h"
#include "tab.h"
namespace neemo { 
    class editor 
    {
        public:
            editor(): m_tabs({}), m_curr_tab(0){}
            void init();
            void run();
            void terminate();
        private:
            std::vector<tab*> m_tabs;
            int m_curr_tab;
            timer m_timer;;
        private:
            char get_char();
            void render_tabs();
            void render_tab(int i);
            void render_selected();
            void render_init();
        private: // low level useless
            termios _mold_terminal;
            termios _mnew_terminal;
    };
}
#endif
