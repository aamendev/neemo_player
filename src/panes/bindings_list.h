#ifndef BINDINGS_LIST_H
#define BINDINGS_LIST_H
#include "../pane.h"
#include "src/common.h"
namespace neemo { namespace panes { 
    class bindings_list : public pane 
    {
        public:
            bindings_list(int parent_row, int parent_col);
            inline virtual void normal_mode(char c) override{};
            inline virtual void edit_mode(char c) override{};
            virtual void render() override;
    };
}}
#endif
