#ifndef PANE_H
#define PANE_H
#include "src/common.h"
#include <iostream>
#include <vector>
namespace neemo 
{
    class pane 
    {
        public:
            virtual ~pane() = default;
            virtual void normal_mode(char c) = 0;
            virtual void edit_mode(char c) = 0;
            virtual void render() = 0;
            virtual void tick() {}
        public:
            float m_percentage = 1.;
            std::vector<std::string> m_elements;
            int m_curr_active_element = 0;
            int m_parent_row = DEFAULT_PARENT_ROW + 1;
            int m_parent_col = DEFAULT_PARENT_COL;
    };
}
#endif
