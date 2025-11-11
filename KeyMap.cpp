// KeyMap.cpp
#include "KeyMap.h"

// własne kody, poza zakresem VK_*, żeby nie kolidowały z klawiaturą
static constexpr WPARAM VK_MOUSE_X1_SYN = 0x1001;
static constexpr WPARAM VK_MOUSE_X2_SYN = 0x1002;

//statyczna mapa wszystkich przycisków
const QList<QPair<QString, WPARAM>> &KeyMap::getOrderedList()
{
    static const QList<QPair<QString, WPARAM>> keyList
        = {// Cyfry i litery
           {"0", '0'},
           {"1", '1'},
           {"2", '2'},
           {"3", '3'},
           {"4", '4'},
           {"5", '5'},
           {"6", '6'},
           {"7", '7'},
           {"8", '8'},
           {"9", '9'},
           {"A", 'A'},
           {"B", 'B'},
           {"C", 'C'},
           {"D", 'D'},
           {"E", 'E'},
           {"F", 'F'},
           {"G", 'G'},
           {"H", 'H'},
           {"I", 'I'},
           {"J", 'J'},
           {"K", 'K'},
           {"L", 'L'},
           {"M", 'M'},
           {"N", 'N'},
           {"O", 'O'},
           {"P", 'P'},
           {"Q", 'Q'},
           {"R", 'R'},
           {"S", 'S'},
           {"T", 'T'},
           {"U", 'U'},
           {"V", 'V'},
           {"W", 'W'},
           {"X", 'X'},
           {"Y", 'Y'},
           {"Z", 'Z'},

           // Numpad
           {"Numpad 0", VK_NUMPAD0},
           {"Numpad 1", VK_NUMPAD1},
           {"Numpad 2", VK_NUMPAD2},
           {"Numpad 3", VK_NUMPAD3},
           {"Numpad 4", VK_NUMPAD4},
           {"Numpad 5", VK_NUMPAD5},
           {"Numpad 6", VK_NUMPAD6},
           {"Numpad 7", VK_NUMPAD7},
           {"Numpad 8", VK_NUMPAD8},
           {"Numpad 9", VK_NUMPAD9},
           {"Numpad .", VK_DECIMAL},
           {"Numpad +", VK_ADD},
           {"Numpad -", VK_SUBTRACT},
           {"Numpad *", VK_MULTIPLY},
           {"Numpad /", VK_DIVIDE},

           // Funkcyjne
           {"F1", VK_F1},
           {"F2", VK_F2},
           {"F3", VK_F3},
           {"F4", VK_F4},
           {"F5", VK_F5},
           {"F6", VK_F6},
           {"F7", VK_F7},
           {"F8", VK_F8},
           {"F9", VK_F9},
           {"F10", VK_F10},
           {"F11", VK_F11},
           {"F12", VK_F12},

           // Specjalne
           {"Esc", VK_ESCAPE},
           {"Tab", VK_TAB},
           {"Backspace", VK_BACK},
           {"Enter", VK_RETURN},
           {"Space", VK_SPACE},
           {"Caps Lock", VK_CAPITAL},
           {"Left Shift", VK_LSHIFT},
           {"Right Shift", VK_RSHIFT},
           {"Left Ctrl", VK_LCONTROL},
           {"Right Ctrl", VK_RCONTROL},
           {"Left Alt", VK_LMENU},
           {"Right Alt", VK_RMENU},
           {"Insert", VK_INSERT},
           {"Delete", VK_DELETE},
           {"Home", VK_HOME},
           {"End", VK_END},
           {"Page Up", VK_PRIOR},
           {"Page Down", VK_NEXT},
           {"Left Arrow", VK_LEFT},
           {"Right Arrow", VK_RIGHT},
           {"Up Arrow", VK_UP},
           {"Down Arrow", VK_DOWN},
           {"Print Screen", VK_SNAPSHOT},
           {"Scroll Lock", VK_SCROLL},
           {"Pause", VK_PAUSE},

           // Znaki specjalne
           {"; :", VK_OEM_1},
           {"/ ?", VK_OEM_2},
           {"~ `", VK_OEM_3},
           {"[ {", VK_OEM_4},
           {"\\ |", VK_OEM_5},
           {"] }", VK_OEM_6},
           {"' \"", VK_OEM_7},
           {"+ =", VK_OEM_PLUS},
           {", <", VK_OEM_COMMA},
           {"- _", VK_OEM_MINUS},
           {". >", VK_OEM_PERIOD},

           // HOTKEYE MYSZY (tylko do wyzwalania, NIE do wysyłania)
           {"Mouse X1", VK_MOUSE_X1_SYN},
           {"Mouse X2", VK_MOUSE_X2_SYN}};

    return keyList;
}

//QHash mapa z ustawionymi na duże litery znakami
const QHash<QString, WPARAM> &KeyMap::getMap()
{
    static QHash<QString, WPARAM> map;
    if (map.isEmpty()) {
        for (const auto &pair : getOrderedList()) {
            map.insert(pair.first.toUpper(), pair.second);
        }
    }
    return map;
}

//Zwraca czytelną wartość klawisza np do logów
QString KeyMap::getKeyText(WPARAM vkCode)
{
    for (const auto &pair : getOrderedList()) {
        if (pair.second == vkCode)
            return pair.first;
    }
    return QString("VK_%1").arg(vkCode);
}
