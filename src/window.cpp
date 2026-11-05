#include "PMGL/window.hpp"
#include <iostream>
#include <PMGL/base.hpp>


/*
====================================================================================
                ANONIMUS NAMESPACE WHIT UTILITY FUNCTION FOR THE CLASS
====================================================================================
*/
namespace{
    /*
        TRANSLATION FUNCTION
    */

    PMGL::key_data translate_key(int key){
        if(key >= GLFW_KEY_A && key <= GLFW_KEY_Z){
            return static_cast<PMGL::key_data>(static_cast<int>(PMGL::key_data::A) + (key - GLFW_KEY_A));
        }else if(key >= GLFW_KEY_0 && key <= GLFW_KEY_9){
            return static_cast<PMGL::key_data>(static_cast<int>(PMGL::key_data::Num0) + (key - GLFW_KEY_0));
        }else{
            return PMGL::key_data::unknown;
        }
    }

    PMGL::mouse_button translate_button(int button){
        if(button >= GLFW_MOUSE_BUTTON_1 && button <= GLFW_MOUSE_BUTTON_8){
            return static_cast<PMGL::mouse_button>(static_cast<int>(PMGL::mouse_button::left) + (button - GLFW_MOUSE_BUTTON_1));
        }
        return PMGL::mouse_button::unknown;
    }

    PMGL::key_mods translate_mod(int mod){
        PMGL::key_mods ret = PMGL::key_mods::none;

        if(mod & GLFW_MOD_ALT){
            ret = ret | PMGL::key_mods::alt;
        }
        if(mod & GLFW_MOD_CAPS_LOCK){
            ret = ret | PMGL::key_mods::caps;
        }
        if(mod & GLFW_MOD_CONTROL){
            ret = ret | PMGL::key_mods::ctrl;
        }
        if(mod & GLFW_MOD_NUM_LOCK){
            ret = ret | PMGL::key_mods::num;
        }
        if(mod & GLFW_MOD_SHIFT){
            ret = ret | PMGL::key_mods::shift;
        }
        if(mod & GLFW_MOD_SUPER){
            ret = ret | PMGL::key_mods::super;
        }
        return ret;
    }

    PMGL::action translate_action(int action){
        switch(action){
            case GLFW_PRESS:
            return PMGL::action::pressed;
            
            case GLFW_RELEASE:
            return PMGL::action::released;

            case GLFW_REPEAT:
            return PMGL::action::repeat;

            default:
            return PMGL::action::unknown;
        }
    }
};



/*
===========================================================================================
                            FORWARD DELARATION FOR HANDLE THE WINDOW
===========================================================================================
*/
class PMGL::window::handle_window{
    private:
    GLFWwindow *handle;

    public:
    handle_window() : handle(nullptr){}

    handle_window(handle_window&) = delete;
    handle_window& operator=(handle_window&) = delete;
    handle_window(handle_window&&) = delete;
    handle_window& operator=(handle_window&&) = delete;
    
    ~handle_window(){
        glfwDestroyWindow(handle);
    }

    handle_window &operator=(GLFWwindow* _w){
        if(_w == handle) return *this;
        glfwDestroyWindow(handle);
        handle = _w;
        return *this;
    }

    operator GLFWwindow*() const{
        return handle;
    }

    friend bool operator==(const handle_window& h1, const handle_window& h2){
        return h1.handle == h2.handle;
    }
    
    friend bool operator!=(const handle_window& h1, const handle_window& h2){
        return h1.handle != h2.handle;
    }
};


/*
===========================================================================================
                            PRIVATE METHODS
===========================================================================================
*/

/*
    UTILITY METHODS
*/
void PMGL::window::init_window(){
    handle = new handle_window();
    *handle = glfwCreateWindow(width, heigth, name.c_str(), NULL, NULL);
    if(static_cast<GLFWwindow *>(*handle) == nullptr){
        std::cout << "Failed to create the window";
        terminate_PMGL();
        exit(-1);
    }
    active();
    glfwSetWindowUserPointer(*handle, this);
    setup_callback();
}

void PMGL::window::setup_callback(){
    //Resize callback
    glfwSetFramebufferSizeCallback(*handle, [](GLFWwindow *low_win, int width, int heigth){
        window *win = static_cast<window*>(glfwGetWindowUserPointer(low_win));
        win->resize_callback(width, heigth);
        win->push_event(event(event_type::RESIZE, vector2d(width, heigth)));
    });

    //Cursor position callback
    glfwSetCursorPosCallback(*handle, [](GLFWwindow *low_win, double xpos, double ypos){
        window *win = static_cast<window *>(glfwGetWindowUserPointer(low_win));
        vector2d v2(xpos, ypos);
        win->push_event(event(event_type::MOUSE_MOVE, v2));
    });


    //Key Traduction Callback
    glfwSetKeyCallback(*handle, [](GLFWwindow* low_win, int _key, int _scancode, int _action, int _mods){
        window* win = static_cast<window *>(glfwGetWindowUserPointer(low_win));
        key_data data = translate_key(_key);
        key_mods mods = translate_mod(_mods);
        action a = translate_action(_action);
        win->push_event(event(keyboard_event(data, mods, a)));
    });

    //Mouse Button Callback
    glfwSetMouseButtonCallback(*handle, [](GLFWwindow* low_win, int _button, int _action, int _mods){
        window* win = static_cast<window *>(glfwGetWindowUserPointer(low_win));
        mouse_button data = translate_button(_button);
        key_mods mods = translate_mod(_mods);
        action a = translate_action(_action);
        win->push_event(event(mouse_button_event(data, mods, a)));
    });
    
    //Mouse Scroll Callback
    glfwSetScrollCallback(*handle, [](GLFWwindow *low_win, double xoffeset, double yoffset){
        window* win = static_cast<window *>(glfwGetWindowUserPointer(low_win));
        vector2d scroll(xoffeset, yoffset);
        win->push_event(event(event_type::MOUSE_SCROLL, scroll));
    });

    //Focus Callback
    glfwSetWindowFocusCallback(*handle, [](GLFWwindow *low_win, int _focus){
        window* win = static_cast<window *>(glfwGetWindowUserPointer(low_win));
        win->set_focus(static_cast<bool>(_focus));
        if(!_focus){
            win->input_handler.release_all();
        }
    });
}


void PMGL::window::push_event(const event &_e){
    event_vector.push_back(_e);
    input_handler.push_input(_e);
}

/*
    UTILITY FOR THE CALLBACKS
*/
void PMGL::window::resize_callback(int w, int h){
    active();
    width = w;
    heigth = h;
    glViewport(0, 0, w, h);
}

void PMGL::window::set_focus(bool _focus){
    focus = _focus;
}



/*
===========================================================================================
                            COSTRUCTOR AND DESTRUCTOR
===========================================================================================
*/
PMGL::window::window(int _w, int _h, std::string_view _name, window_states _state, handle_window  *_context) 
                    : width(_w), heigth(_h), name(_name), state(_state)
{
    init_window();
}

PMGL::window::~window(){
    delete handle;
}





/*
===========================================================================================
                            PUBLIC METHODS
===========================================================================================
*/

/*
    Handling
*/

void PMGL::window::active() const{
    glfwMakeContextCurrent(*handle);
}

bool PMGL::window::should_close() const{
    return glfwWindowShouldClose(*handle);
}

bool PMGL::window::is_on_focus() const{
    return focus;
}

void PMGL::window::end_frame(){
    glfwSwapBuffers(*handle);
}

const PMGL::window::handle_window *PMGL::window::share_context() const{
    return handle;
}




/*
    SETTERS
*/
void PMGL::window::set_vsync(int value){
    active();
    glfwSwapInterval(value);
}

void PMGL::window::set_cursor_mode(PMGL::cursor_mode mode){
    switch(mode){
        case PMGL::cursor_mode::captured:
        glfwSetInputMode(*handle, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
        break;
        case PMGL::cursor_mode::disabled:
        glfwSetInputMode(*handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
        case PMGL::cursor_mode::hidden:
        glfwSetInputMode(*handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        break;
        case PMGL::cursor_mode::normal:
        glfwSetInputMode(*handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
        case PMGL::cursor_mode::unavailable:
        default:
        glfwSetInputMode(*handle, GLFW_CURSOR, GLFW_CURSOR_UNAVAILABLE);
    }
}


/*
    INPUTS
*/
void PMGL::window::pollEvents(){
    active();
    input_handler.clear_input();
    event_vector.clear();
    glfwPollEvents();
}

const PMGL::Input &PMGL::window::getInput(){
    return input_handler;
}




