#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <list>

#include <SFML/Window.hpp>
#include <SFML/System/Mutex.hpp>

namespace sw
{
    class Action
    {
    public:
        Action() = default;
        
    public:
        enum eventType
        {
            keyPressed,
            keyReleased,
            mouseMoved,
            mouseMovedFromCenter,
            mouseButtonPressed,
            receivedFocus,
            lostFocus,
            mouseEntered,
            mouseLeft,
        };
        

        struct KeyEvent
        {
            sf::Keyboard::Key code;
        };

        struct MouseMoveEvent
        {
            unsigned x;
            unsigned y;
        };

        struct MouseButtonEvent
        {
            sf::Mouse::Button button;
            int x;
            int y;
        };

    public:
        eventType type;

    union
    {
        KeyEvent key;
        MouseMoveEvent mouse;
        MouseButtonEvent button;
    };

    };

    // event handling will be devided into two sections :
    // general event handling and program specific event handling, 
    // general event handling is used for used to handle basic events like gaining focus, losing focus,
    // window size changing, these events are handled in the main thread, program specific event handling
    // provides an extra layer of abstraction for the application that saves us the hurdle of dealing with
    // basic event and interpreting their meaning to the program.
    class EventHandler
    {
    public:
        struct AsyncState // state for the main thread
        {
            bool hasFocus {false}; // to implement send messages only if you are focused (as an option)
            bool centerMouse {false}; // put mouse to center 
            unsigned width; // needed for inmplementing the above
            unsigned height; // same as above
            sf::Vector2u center;
        };

        struct Controls // last known state of keys in the frame
        {
            bool right;
            bool left;
            bool up;
            bool down;
        };

        struct State // state for the reder thread
        {
            bool Focus;

        };

        struct FrameInputData // frame events (did something happen / what is the sum of the happenings of some kind)
        {
            bool escape;
            bool mouseInsideClientArea;
            bool centerMouse;
            sf::Vector2i mouseDeviation;
        };

    public:
        EventHandler(sf::Window& window, unsigned width, unsigned height);
        void centerMouse(bool);
        void handleEvent(sf::Event e); //provides immediate Event handling that we can't afford to delay like setting the mouse back to the middle of the screen, call this function in the main thread
        void addAction(Action&);
        void fetchActions();
        void processActions();
        Controls getControls();
        FrameInputData getFrameInputData();

    private:
        sf::Window& mWindow;
        std::list<Action> pendingActions; // queue safe to access from the main thread
        std::list<Action> currentActions; // queue safe to access from the render thread
        sf::Mutex mutex;
        AsyncState asyncState;
        State state;
        FrameInputData frameInputData;
        Controls controls {};

    
    public:
        decltype(EventHandler::currentActions)& getActions(); // don't use this 
    };

}


#endif