#ifndef COMMON_H
#define COMMON_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>
#define DEFAULT_PARENT_COL (0)
#define DEFAULT_PARENT_ROW (5)
#define DEFAULT_OFFSET_COL (3)
#define DEFAULT_OFFSET_ROW (3)
#include <chrono>
namespace neemo 
{
    enum class command_mode
    {
        normal,
        edit,
    };
    struct editor_settings 
    {
        bool should_close = false;
        int width = 1920;
        int height = 1080;
        command_mode mode = command_mode::normal;
        std::queue<std::string> song_queue;
    };
    inline int decrement_and_wrap(int x, int max, int min = 0)
    {
        return (x - 1) * (x > min) + (max) * !(x>min);
    }
    inline editor_settings g_editor_settings;
    inline void set_cursor(int row, int col, int parent_row = 0, int parent_col = 0)
    {
        std::cout << "\033[" << parent_row + row << ";" << parent_col + col << "H";
    }
    inline void reset_col()
    {
        std::cout << "\033[0m";
    }
    inline void set_bg(int r, int g, int b)
    {
        std::cout << "\033[48;2;" + 
            std::to_string(r) + ";" +
            std::to_string(g) + ";" +
            std::to_string(b) + ";m";
    }
    inline void set_fg(int r, int g, int b)
    {
        std::cout << "\033[38;2;" + 
            std::to_string(r) + ";" +
            std::to_string(g) + ";" +
            std::to_string(b) + ";m";
    }
    inline void write_with_bg(std::string str, int r, int g, int b)
    {
        set_bg(r, g, b);
        std::cout << str;
        reset_col();
    }
    inline void write_with_fg(std::string str, int r, int g, int b)
    {
        set_fg(r, g, b);
        std::cout << str;
        reset_col();
    }
    inline void write_with_fg_bg(std::string str, 
            int fg_r, int fg_g, int fg_b,
            int bg_r, int bg_g, int bg_b
            )
    {
        set_fg(fg_r, fg_g, fg_b);
        set_bg(bg_r, bg_g, bg_b);
        std::cout << str;
        reset_col();
    }
    struct colour
    {
        int r;
        int g;
        int b;
    };
    inline colour selected_bg{.r = 42, .g = 22, .b = 42};
    inline colour selected_fg{.r = 206, .g = 163, .b = 118};

    enum class timer_unit
    {
        us = 1000000,
        ms = 1000,
        s = 1,
    };
    class timer
    {
        public:
            inline void begin() {mstart = std::chrono::steady_clock::now();}
            inline void end() {mend = std::chrono::steady_clock::now();};
            inline float get_time(timer_unit tu = timer_unit::ms) 
            {return ((std::chrono::duration<float>(mend-mstart)).count()) * (float)(tu);};
            inline void wait(float t, timer_unit tu = timer_unit::ms)
            {
                if (t < 0) return;
                auto init_time = std::chrono::high_resolution_clock::now();
                auto curr_time = std::chrono::high_resolution_clock::now();
                do 
                {
                    curr_time = std::chrono::high_resolution_clock::now();
                } while ((curr_time - init_time).count() * (float)tu < t);
            }
        private:
            std::chrono::time_point<std::chrono::steady_clock> mstart;
            std::chrono::time_point<std::chrono::steady_clock> mend;
    };

    inline void run_system(std::string command)
    {
        auto str = command + " > /dev/null 2>&1";
        std::system(str.c_str());
    }
}
#endif
