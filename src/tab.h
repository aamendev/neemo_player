#ifndef TAB_H
#define TAB_H
#include "common.h"
#include "pane.h"
#include <vector>
namespace neemo 
{
    class tab
    {
        public:
            virtual ~tab() = default;
            inline void init()
            {
                float sum = 0.;
                for (auto p : m_panes)
                {
                    sum += p->m_percentage;
                };
                for (auto p : m_panes)
                {
                    p->m_percentage /= sum;
                };
            }
            virtual void activate() = 0;
            virtual void normal_mode(char c) = 0;
            virtual void edit_mode(char c) = 0;
            virtual void tick() {}
            std::string m_name = "";
            std::vector<pane*> m_panes;
        protected:
            int m_curr_active_pane = 0;
    };
}
#endif
