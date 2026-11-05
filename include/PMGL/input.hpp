#ifndef INPUT_HPP
#define INPUT_HPP
#include "PMGL/base.hpp"
#include "PMGL/event.hpp"
#include <vector>
#include <span>
#include <bitset>

namespace PMGL{
    
    class Input{
        private:
        //Data fields
        //Keyboard
        std::bitset<static_cast<size_t>(key_data::keyCount)> previous_keys;
        std::bitset<static_cast<size_t>(key_data::keyCount)> current_keys;
        
        //Mouse button
        std::bitset<static_cast<size_t>(mouse_button::buttonCount)> previous_button;
        std::bitset<static_cast<size_t>(mouse_button::buttonCount)> current_button;

        vector2d mouse_pos;
        vector2d mouse_delta;
        vector2d mouse_scrollDelta;

        //Util methods
        static size_t index(key_data k);
        static size_t index(mouse_button m);

        //Private methods for push event
        void update_currentkeys(const event& _e);
        void update_currentButton(const event& _e);
        void set_mousePosition(const event& _e);
        void set_mouseScroll(const event& _e);
        
        public:
        //Costructor
        Input();
        
        //Public methods for the user
        bool isKeyDown(key_data key) const;
        bool isKeyPressed(key_data key) const;
        bool isKeyReleased(key_data key) const;
        const vector2d& mousePosition() const;
        const vector2d& mouseDelta() const;
        const vector2d& mouseScroll() const;
        
        //Public methods for the window
        void push_input(const event& _e);
        void clear_input();
        void release_all();
        
    };
}
#endif