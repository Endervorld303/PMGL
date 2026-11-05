#ifndef EVENT_HPP
#define EVENT_HPP
#include <cstdint>
#include "PMGL/base.hpp"

namespace PMGL{
    
    /*
    ===============================
        ENUM
    ===============================
    */
    enum class event_type{
        UNKNOWN = 0,
        KEYBOARD,
        MOUSE_BUTTON,
        MOUSE_MOVE,
        MOUSE_SCROLL,
        CHAR_INSERT,
        RESIZE,
        CLOSE_REQUEST
    };

    enum class key_data{
        unknown = 0,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        
        keyCount
    };

    enum class key_mods : std::uint8_t{
        none = 0,
        shift = 1 << 0,
        alt = 1 << 1,
        ctrl = 1 << 2,
        super = 1 << 3,
        caps = 1 << 4,
        num = 1 << 5
    };
    key_mods operator|(key_mods k1, key_mods k2);
    key_mods operator&(key_mods k1, key_mods k2);

    enum class action{
        unknown = 0,
        pressed,
        released,
        repeat
    };

    enum class mouse_button{
        unknown = 0,
        left, right, middle, button4, button5, button6, button7, button8,

        buttonCount
    };


    /*
    ========================================
        EVENT CLASSES
    ========================================

    */
    
    class keyboard_event{
        private:
        key_data value;
        key_mods mods;
        action state;

        public:
        keyboard_event(key_data _v, key_mods _m, action _state);
        friend bool operator==(const keyboard_event&, const keyboard_event&);
        friend bool operator!=(const keyboard_event&, const keyboard_event&);
        key_data getKey() const;
        key_mods getMods() const;
        action getState() const;
    };

    class mouse_button_event{
        private:
        mouse_button value;
        key_mods mods;
        action state;

        public:
        mouse_button_event(mouse_button _value, key_mods _mods, action _state);
        friend bool operator==(const mouse_button_event& , const mouse_button_event&);
        friend bool operator!=(const mouse_button_event&, const mouse_button_event&);
        mouse_button getButton() const;
        key_mods getMods() const;
        action getState() const;
    };



    /*
        COMPLETE EVENT CLASS
    */
    class event{
        private:
        event_type type;
        union event_payload{
            event_payload() : other(0.0f, 0.0f){};
            explicit event_payload(const keyboard_event& _key) : keyboard(_key){}
            explicit event_payload(const mouse_button_event& _mouse) : mouse_button(_mouse){}
            explicit event_payload(const vector2d& _other) : other(_other){}
            vector2d other;
            keyboard_event keyboard;
            mouse_button_event mouse_button;
        }data;

        public:
        event(event_type _type = event_type::UNKNOWN, vector2d _data = vector2d(0.0f, 0.0f));
        event(const keyboard_event& _data);
        event(const mouse_button_event& _data);

        const keyboard_event& getKeyboardData() const;
        const mouse_button_event& geMouseButtontData() const;
        event_type getType() const;
        const vector2d& getVector2d() const;
        
        friend bool operator==(const event&, const event&);
        friend bool operator==(const event&, event_type);
        friend bool operator!=(const event&, const event&);
    };
}


#endif