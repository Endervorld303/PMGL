#include "PMGL/input.hpp"

/*
    PRIVATE METHODS
*/
#include <iostream>

void PMGL::Input::set_mousePosition(const event& _e){
    mouse_delta = mouse_delta + (_e.getVector2d()  - mouse_pos);
    mouse_pos = _e.getVector2d();
}

void PMGL::Input::set_mouseScroll(const event& _e){
    mouse_scrollDelta = mouse_scrollDelta + _e.getVector2d();
}

void PMGL::Input::push_input(const event &_e){
    switch(_e.getType()){
        case event_type::KEYBOARD:
        update_currentkeys(_e);
        break;
        
        case event_type::MOUSE_BUTTON:
        update_currentButton(_e);
        break;

        case event_type::MOUSE_MOVE:
        set_mousePosition(_e);
        break;

        case event_type::MOUSE_SCROLL:
        set_mouseScroll(_e);
        break;
    }
}

void PMGL::Input::update_currentkeys(const event &_e){
    keyboard_event k = _e.getKeyboardData();
    key_data key_value = k.getKey();
    if(key_value == key_data::unknown || key_value >= key_data::keyCount){
        return;
    }
    
    switch(k.getState()){
        case action::pressed:
        current_keys.set(index(key_value));
        break;
        case action::released:
        current_keys.reset(index(key_value));
        break;
    }
}

void PMGL::Input::update_currentButton(const event &_e){
    mouse_button_event m = _e.geMouseButtontData();
    mouse_button button_value = m.getButton();
    if(button_value == mouse_button::unknown || button_value >= mouse_button::buttonCount){
        return;
    }

    switch(m.getState()){
        case action::pressed:
        current_button.set(index(button_value));
        break;
        case action::released:
        current_button.reset(index(button_value));
        break;
    }
}


void PMGL::Input::clear_input(){
    mouse_delta = 0.0f;
    mouse_scrollDelta = 0.0f;
    previous_keys = current_keys;
    previous_button = current_button;
}

void PMGL::Input::release_all(){
    current_button.reset();
    current_keys.reset();
}

size_t PMGL::Input::index(key_data k){
    if(k >= key_data::keyCount || k <= key_data::unknown){ 
        k = key_data::unknown;
    }
    return static_cast<size_t>(k);
}

size_t PMGL::Input::index(mouse_button m){
    return static_cast<size_t>(m);
}

/*
    COSTUCTOR
*/
PMGL::Input::Input() : mouse_pos(0.0f,0.0f), mouse_delta(0.0f, 0.0f), mouse_scrollDelta(0.0f, 0.0f){}


/*
    PUBLIC METHODS
*/
bool PMGL::Input::isKeyDown(key_data key) const{
    return current_keys[index(key)];
}

bool PMGL::Input::isKeyPressed(key_data key) const{
    return current_keys[index(key)] && !previous_keys[index(key)];
}

bool PMGL::Input::isKeyReleased(key_data key) const{
    return !current_keys[index(key)] && previous_keys[index(key)];
}

const PMGL::vector2d& PMGL::Input::mousePosition() const{
    return mouse_pos;
}

const PMGL::vector2d &PMGL::Input::mouseDelta() const{
    return mouse_delta;
}

const PMGL::vector2d &PMGL::Input::mouseScroll() const{
    return mouse_scrollDelta;
}
