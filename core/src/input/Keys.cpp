#include "pixl/core/input/Keys.h"
#include "pixl/core/window/Window.h"

using namespace px;

KeyEntry px::Keys::SPACE = {};
KeyEntry px::Keys::APOSTROPHE = {};
KeyEntry px::Keys::COMMA = {};
KeyEntry px::Keys::MINUS = {};
KeyEntry px::Keys::PERIOD = {};
KeyEntry px::Keys::SLASH = {};
KeyEntry px::Keys::ZERO = {};
KeyEntry px::Keys::ONE = {};
KeyEntry px::Keys::TWO = {};
KeyEntry px::Keys::THREE = {};
KeyEntry px::Keys::FOUR = {};
KeyEntry px::Keys::FIVE = {};
KeyEntry px::Keys::SIX = {};
KeyEntry px::Keys::SEVEN = {};
KeyEntry px::Keys::EIGHT = {};
KeyEntry px::Keys::NINE = {};
KeyEntry px::Keys::SEMICOLON = {};
KeyEntry px::Keys::EQUAL = {};
KeyEntry px::Keys::A = {};
KeyEntry px::Keys::B = {};
KeyEntry px::Keys::C = {};
KeyEntry px::Keys::D = {};
KeyEntry px::Keys::E = {};
KeyEntry px::Keys::F = {};
KeyEntry px::Keys::G = {};
KeyEntry px::Keys::H = {};
KeyEntry px::Keys::I = {};
KeyEntry px::Keys::J = {};
KeyEntry px::Keys::K = {};
KeyEntry px::Keys::L = {};
KeyEntry px::Keys::M = {};
KeyEntry px::Keys::N = {};
KeyEntry px::Keys::O = {};
KeyEntry px::Keys::P = {};
KeyEntry px::Keys::Q = {};
KeyEntry px::Keys::R = {};
KeyEntry px::Keys::S = {};
KeyEntry px::Keys::T = {};
KeyEntry px::Keys::U = {};
KeyEntry px::Keys::V = {};
KeyEntry px::Keys::W = {};
KeyEntry px::Keys::X = {};
KeyEntry px::Keys::Y = {};
KeyEntry px::Keys::Z = {};
KeyEntry px::Keys::LEFT_BRACKET = {};
KeyEntry px::Keys::BACKSLASH = {};
KeyEntry px::Keys::RIGHT_BRACKET = {};
KeyEntry px::Keys::GRAVE_ACCENT = {};
KeyEntry px::Keys::WORLD_1 = {};
KeyEntry px::Keys::WORLD_2 = {};
KeyEntry px::Keys::ESCAPE = {};
KeyEntry px::Keys::ENTER = {};
KeyEntry px::Keys::TAB = {};
KeyEntry px::Keys::BACKSPACE = {};
KeyEntry px::Keys::INSERT = {};
KeyEntry px::Keys::DELETE = {};
KeyEntry px::Keys::RIGHT = {};
KeyEntry px::Keys::LEFT = {};
KeyEntry px::Keys::DOWN = {};
KeyEntry px::Keys::UP = {};
KeyEntry px::Keys::PAGE_UP = {};
KeyEntry px::Keys::PAGE_DOWN = {};
KeyEntry px::Keys::HOME = {};
KeyEntry px::Keys::END = {};
KeyEntry px::Keys::CAPS_LOCK = {};
KeyEntry px::Keys::SCROLL_LOCK = {};
KeyEntry px::Keys::NUM_LOCK = {};
KeyEntry px::Keys::PRINT_SCREEN = {};
KeyEntry px::Keys::PAUSE = {};
KeyEntry px::Keys::F1 = {};
KeyEntry px::Keys::F2 = {};
KeyEntry px::Keys::F3 = {};
KeyEntry px::Keys::F4 = {};
KeyEntry px::Keys::F5 = {};
KeyEntry px::Keys::F6 = {};
KeyEntry px::Keys::F7 = {};
KeyEntry px::Keys::F8 = {};
KeyEntry px::Keys::F9 = {};
KeyEntry px::Keys::F10 = {};
KeyEntry px::Keys::F11 = {};
KeyEntry px::Keys::F12 = {};
KeyEntry px::Keys::F13 = {};
KeyEntry px::Keys::F14 = {};
KeyEntry px::Keys::F15 = {};
KeyEntry px::Keys::F16 = {};
KeyEntry px::Keys::F17 = {};
KeyEntry px::Keys::F18 = {};
KeyEntry px::Keys::F19 = {};
KeyEntry px::Keys::F20 = {};
KeyEntry px::Keys::F21 = {};
KeyEntry px::Keys::F22 = {};
KeyEntry px::Keys::F23 = {};
KeyEntry px::Keys::F24 = {};
KeyEntry px::Keys::F25 = {};
KeyEntry px::Keys::KP_0 = {};
KeyEntry px::Keys::KP_1 = {};
KeyEntry px::Keys::KP_2 = {};
KeyEntry px::Keys::KP_3 = {};
KeyEntry px::Keys::KP_4 = {};
KeyEntry px::Keys::KP_5 = {};
KeyEntry px::Keys::KP_6 = {};
KeyEntry px::Keys::KP_7 = {};
KeyEntry px::Keys::KP_8 = {};
KeyEntry px::Keys::KP_9 = {};
KeyEntry px::Keys::KP_DECIMAL = {};
KeyEntry px::Keys::KP_DIVIDE = {};
KeyEntry px::Keys::KP_MULTIPLY = {};
KeyEntry px::Keys::KP_SUBTRACT = {};
KeyEntry px::Keys::KP_ADD = {};
KeyEntry px::Keys::KP_ENTER = {};
KeyEntry px::Keys::KP_EQUAL = {};
KeyEntry px::Keys::LEFT_SHIFT = {};
KeyEntry px::Keys::LEFT_CONTROL = {};
KeyEntry px::Keys::LEFT_ALT = {};
KeyEntry px::Keys::LEFT_SUPER = {};
KeyEntry px::Keys::RIGHT_SHIFT = {};
KeyEntry px::Keys::RIGHT_CONTROL = {};
KeyEntry px::Keys::RIGHT_ALT = {};
KeyEntry px::Keys::RIGHT_SUPER = {};
KeyEntry px::Keys::MENU = {};
KeyEntry px::Keys::LAST = {};

static std::unordered_map<KeyCode, KeyEntry*> entries;

static bool OnKeyPress(const EventData& data, void* ignored)
{
    const KeyEvent& event = (const KeyEvent&)data;

    if (entries.count(event.code) < 1) return true;

    entries.at(event.code)->justPressed = true;
    entries.at(event.code)->pressed = true;

    return true;
}

static bool OnKeyRelease(const EventData& data, void* ignored)
{
    const KeyEvent& event = (const KeyEvent&)data;

    if (entries.count(event.code) < 1) return true;

    entries.at(event.code)->justReleased = true;
    entries.at(event.code)->pressed = false;

    return true;
}

void px::Keys::Init(Window* rootWnd)
{
    rootWnd->GetEventManager()->AddListener("__pixl_keys_press", PX_EVENT(KEY_PRESS), OnKeyPress, ListenerPriority::HIGH);
    rootWnd->GetEventManager()->AddListener("__pixl_keys_release", PX_EVENT(KEY_RELEASE), OnKeyRelease, ListenerPriority::HIGH);

    entries.insert({ KeyCode::SPACE, &SPACE });
    entries.insert({ KeyCode::APOSTROPHE, &APOSTROPHE });
    entries.insert({ KeyCode::COMMA, &COMMA });
    entries.insert({ KeyCode::MINUS, &MINUS });
    entries.insert({ KeyCode::PERIOD, &PERIOD });
    entries.insert({ KeyCode::SLASH, &SLASH });
    entries.insert({ KeyCode::ZERO, &ZERO });
    entries.insert({ KeyCode::ONE, &ONE });
    entries.insert({ KeyCode::TWO, &TWO });
    entries.insert({ KeyCode::THREE, &THREE });
    entries.insert({ KeyCode::FOUR, &FOUR });
    entries.insert({ KeyCode::FIVE, &FIVE });
    entries.insert({ KeyCode::SIX, &SIX });
    entries.insert({ KeyCode::SEVEN, &SEVEN });
    entries.insert({ KeyCode::EIGHT, &EIGHT });
    entries.insert({ KeyCode::NINE, &NINE });
    entries.insert({ KeyCode::SEMICOLON, &SEMICOLON });
    entries.insert({ KeyCode::EQUAL, &EQUAL });
    entries.insert({ KeyCode::A, &A });
    entries.insert({ KeyCode::B, &B });
    entries.insert({ KeyCode::C, &C });
    entries.insert({ KeyCode::D, &D });
    entries.insert({ KeyCode::E, &E });
    entries.insert({ KeyCode::F, &F });
    entries.insert({ KeyCode::G, &G });
    entries.insert({ KeyCode::H, &H });
    entries.insert({ KeyCode::I, &I });
    entries.insert({ KeyCode::J, &J });
    entries.insert({ KeyCode::K, &K });
    entries.insert({ KeyCode::L, &L });
    entries.insert({ KeyCode::M, &M });
    entries.insert({ KeyCode::N, &N });
    entries.insert({ KeyCode::O, &O });
    entries.insert({ KeyCode::P, &P });
    entries.insert({ KeyCode::Q, &Q });
    entries.insert({ KeyCode::R, &R });
    entries.insert({ KeyCode::S, &S });
    entries.insert({ KeyCode::T, &T });
    entries.insert({ KeyCode::U, &U });
    entries.insert({ KeyCode::V, &V });
    entries.insert({ KeyCode::W, &W });
    entries.insert({ KeyCode::X, &X });
    entries.insert({ KeyCode::Y, &Y });
    entries.insert({ KeyCode::Z, &Z });
    entries.insert({ KeyCode::LEFT_BRACKET, &LEFT_BRACKET });
    entries.insert({ KeyCode::BACKSLASH, &BACKSLASH });
    entries.insert({ KeyCode::RIGHT_BRACKET, &RIGHT_BRACKET });
    entries.insert({ KeyCode::GRAVE_ACCENT, &GRAVE_ACCENT });
    entries.insert({ KeyCode::WORLD_1, &WORLD_1 });
    entries.insert({ KeyCode::WORLD_2, &WORLD_2 });
    entries.insert({ KeyCode::ESCAPE, &ESCAPE });
    entries.insert({ KeyCode::ENTER, &ENTER });
    entries.insert({ KeyCode::TAB, &TAB });
    entries.insert({ KeyCode::BACKSPACE, &BACKSPACE });
    entries.insert({ KeyCode::INSERT, &INSERT });
    entries.insert({ KeyCode::DELETE, &DELETE });
    entries.insert({ KeyCode::RIGHT, &RIGHT });
    entries.insert({ KeyCode::LEFT, &LEFT });
    entries.insert({ KeyCode::DOWN, &DOWN });
    entries.insert({ KeyCode::UP, &UP });
    entries.insert({ KeyCode::PAGE_UP, &PAGE_UP });
    entries.insert({ KeyCode::PAGE_DOWN, &PAGE_DOWN });
    entries.insert({ KeyCode::HOME, &HOME });
    entries.insert({ KeyCode::END, &END });
    entries.insert({ KeyCode::CAPS_LOCK, &CAPS_LOCK });
    entries.insert({ KeyCode::SCROLL_LOCK, &SCROLL_LOCK });
    entries.insert({ KeyCode::NUM_LOCK, &NUM_LOCK });
    entries.insert({ KeyCode::PRINT_SCREEN, &PRINT_SCREEN });
    entries.insert({ KeyCode::PAUSE, &PAUSE });
    entries.insert({ KeyCode::F1, &F1 });
    entries.insert({ KeyCode::F2, &F2 });
    entries.insert({ KeyCode::F3, &F3 });
    entries.insert({ KeyCode::F4, &F4 });
    entries.insert({ KeyCode::F5, &F5 });
    entries.insert({ KeyCode::F6, &F6 });
    entries.insert({ KeyCode::F7, &F7 });
    entries.insert({ KeyCode::F8, &F8 });
    entries.insert({ KeyCode::F9, &F9 });
    entries.insert({ KeyCode::F10, &F10 });
    entries.insert({ KeyCode::F11, &F11 });
    entries.insert({ KeyCode::F12, &F12 });
    entries.insert({ KeyCode::F13, &F13 });
    entries.insert({ KeyCode::F14, &F14 });
    entries.insert({ KeyCode::F15, &F15 });
    entries.insert({ KeyCode::F16, &F16 });
    entries.insert({ KeyCode::F17, &F17 });
    entries.insert({ KeyCode::F18, &F18 });
    entries.insert({ KeyCode::F19, &F19 });
    entries.insert({ KeyCode::F20, &F20 });
    entries.insert({ KeyCode::F21, &F21 });
    entries.insert({ KeyCode::F22, &F22 });
    entries.insert({ KeyCode::F23, &F23 });
    entries.insert({ KeyCode::F24, &F24 });
    entries.insert({ KeyCode::F25, &F25 });
    entries.insert({ KeyCode::KP_0, &KP_0 });
    entries.insert({ KeyCode::KP_1, &KP_1 });
    entries.insert({ KeyCode::KP_2, &KP_2 });
    entries.insert({ KeyCode::KP_3, &KP_3 });
    entries.insert({ KeyCode::KP_4, &KP_4 });
    entries.insert({ KeyCode::KP_5, &KP_5 });
    entries.insert({ KeyCode::KP_6, &KP_6 });
    entries.insert({ KeyCode::KP_7, &KP_7 });
    entries.insert({ KeyCode::KP_8, &KP_8 });
    entries.insert({ KeyCode::KP_9, &KP_9 });
    entries.insert({ KeyCode::KP_DECIMAL, &KP_DECIMAL });
    entries.insert({ KeyCode::KP_DIVIDE, &KP_DIVIDE });
    entries.insert({ KeyCode::KP_MULTIPLY, &KP_MULTIPLY });
    entries.insert({ KeyCode::KP_SUBTRACT, &KP_SUBTRACT });
    entries.insert({ KeyCode::KP_ADD, &KP_ADD });
    entries.insert({ KeyCode::KP_ENTER, &KP_ENTER });
    entries.insert({ KeyCode::KP_EQUAL, &KP_EQUAL });
    entries.insert({ KeyCode::LEFT_SHIFT, &LEFT_SHIFT });
    entries.insert({ KeyCode::LEFT_CONTROL, &LEFT_CONTROL });
    entries.insert({ KeyCode::LEFT_ALT, &LEFT_ALT });
    entries.insert({ KeyCode::LEFT_SUPER, &LEFT_SUPER });
    entries.insert({ KeyCode::RIGHT_SHIFT, &RIGHT_SHIFT });
    entries.insert({ KeyCode::RIGHT_CONTROL, &RIGHT_CONTROL });
    entries.insert({ KeyCode::RIGHT_ALT, &RIGHT_ALT });
    entries.insert({ KeyCode::RIGHT_SUPER, &RIGHT_SUPER });
    entries.insert({ KeyCode::MENU, &MENU });
    entries.insert({ KeyCode::LAST, &LAST });
}

void px::Keys::Update()
{
    for (auto& v : entries)
    {
        v.second->justPressed = false;
        v.second->justReleased = false;
    }
}

bool px::Keys::Pressed(KeyCode code)
{
    if (entries.count(code) < 1) return false;
    return entries.at(code)->pressed;
}

bool px::Keys::JustPressed(KeyCode code)
{
    if (entries.count(code) < 1) return false;
    return entries.at(code)->justPressed;
}

bool px::Keys::JustReleased(KeyCode code)
{
    if (entries.count(code) < 1) return false;
    return entries.at(code)->justReleased;
}
