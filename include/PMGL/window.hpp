#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <string_view>

#include "PMGL/input.hpp"



namespace PMGL{


    enum class cursor_mode{
        unavailable = 0, 
        normal,
        hidden,
        captured,
        disabled
    };
    
    enum class window_states{
        WINDOWED,
        FULLSCREEN,
    };

    class window{
        private:
        //Forward declaration for handling the window in various backend
        class handle_window;
        
        //Field of the class
        handle_window* handle;  //Handler of the window
        Input input_handler;
        std::vector<event> event_vector;
        int width;
        int heigth;
        std::string name;
        window_states state;    //Actual state of the window
        bool focus; 

        //Utility function
        void setup_callback();
        void init_window(); //This function initialize the window using the handle_window
        void push_event(const event& _e);
        
        //Callback
        void resize_callback(int w, int h);
        void set_focus(bool _focus);
        
        public:
        //Costructor
        window(int _w, int _h, std::string_view _name, window_states _state = window_states::WINDOWED, handle_window* _context = nullptr);
        window(window &w) = delete;
        window& operator=(const window &w) = delete; 
        window(window &&w) = delete;
        window& operator=(const window &&w) = delete; 
        ~window();

        //Handling
        void active() const;
        bool should_close() const;
        bool is_on_focus() const;
        void end_frame();
        const handle_window* share_context() const;
        
        //Setter
        void set_vsync(int value);
        void set_cursor_mode(cursor_mode mode);
        
        

        //Input
        void pollEvents();
        const Input& getInput();
    };
    
}
#endif