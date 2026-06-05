#ifndef QUEUE_H
#define QUEUE_H
#include "../tab.h"
namespace neemo { namespace tabs{
    class queue : public tab
    {
        public:
        queue(int row, int col);
        inline virtual void normal_mode(char c)override;
        inline virtual void edit_mode(char c) override;
        virtual void activate() override;
        virtual void tick() override;
    };
}}
#endif
