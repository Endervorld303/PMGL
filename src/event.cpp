#include "PMGL/event.hpp"
#include "PMGL/base.hpp"

namespace PMGL{
    /*
        KEY MODS
    */
    key_mods operator|(key_mods k1, key_mods k2){
        return static_cast<key_mods>(static_cast<uint8_t>(k1) | static_cast<uint8_t>(k2));
    }

    key_mods operator&(key_mods k1, key_mods k2){
        return static_cast<key_mods>(static_cast<uint8_t>(k1) & static_cast<uint8_t>(k2));
    }


    /*
    =============================
        KEYBOARD EVENT
    =============================
    */
    keyboard_event::keyboard_event(key_data _value, key_mods _mods, action _state) 
                                    : value(_value), mods(_mods), state(_state) {}

    key_data keyboard_event::getKey() const{
        return value;
    }

    key_mods keyboard_event::getMods() const{
        return mods;
    }

    action keyboard_event::getState() const{
        return state;
    }

    bool operator==(const keyboard_event &key1, const keyboard_event &key2){
        return (key1.value == key2.value) && (key1.mods == key2.mods);
    }

    bool operator!=(const keyboard_event &key1, const keyboard_event &key2){
        return ((key1.value != key2.value) && (key1.mods != key2.mods));
    }




    /*
    ============================
        MOUSE BUTTON EVENT
    ============================
    */

    mouse_button_event::mouse_button_event(mouse_button _value, key_mods _mods, action _state) 
                                                : value(_value), mods(_mods), state(_state){}

    mouse_button mouse_button_event::getButton() const{
        return value;
    }

    key_mods mouse_button_event::getMods() const{
        return mods;
    }

    action mouse_button_event::getState() const{
        return state;
    }

    bool operator==(const mouse_button_event& mouse1, const mouse_button_event& mouse2){
        return (mouse1.value == mouse2.value) && (mouse1.mods == mouse2.mods);
    }

    bool operator!=(const mouse_button_event &mouse1, const mouse_button_event &mouse2){
        return (mouse1.value != mouse2.value) && (mouse1.mods != mouse2.mods);
    }

    

    

    /*
        EVENT COSTRUCTOR
    */
   
    event::event(event_type _type, PMGL::vector2d _data) : type(_type), data(_data){}
    
    event::event( const keyboard_event &_data) : type(event_type::KEYBOARD), data(_data) {}

    event::event(const mouse_button_event &_data) : type(event_type::MOUSE_BUTTON), data(_data) {}
    
    /*
        EVENT OPERATOR
    */
    bool operator==(const event &e1, const event &e2){
        if(e1.type == e2.type){
            switch(e1.type){
                case event_type::KEYBOARD:
                    return e1.data.keyboard == e2.data.keyboard;
                    break;
                case event_type::MOUSE_BUTTON:
                    return e1.data.mouse_button == e2.data.mouse_button;
                    break;

                default:
                    return true;
                    break;
            }
        }
        else{
            return false;
        }
    }
    
    bool operator!=(const event &e1, const event &e2){
        return !(e1 == e2);
    }

    bool operator==(const event &e1, event_type t1){
        return e1.type == t1;
    }


    /*
                    EVENT GETTER
                */
    const keyboard_event &event::getKeyboardData() const{
        return data.keyboard;
    }

    const mouse_button_event &event::geMouseButtontData() const{
        return data.mouse_button;
    }
    event_type event::getType() const{
        return type;
    }
    const vector2d &event::getVector2d() const{
        return data.other;
    }
}