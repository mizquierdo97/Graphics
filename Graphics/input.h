#ifndef INPUT_H
#define INPUT_H

#include <QKeyEvent>

enum class KeyState{Up, Pressed, Down};

enum  class MouseButtonState{Up, Pressed, Down};

class Input
{
public:
    Input();

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void postUpdate();

    enum{
        MAX_BUTTONS = 10,
        MAX_KEYS = 300
    };

    KeyState keys[MAX_KEYS];

    MouseButtonState mouseButtons[MAX_BUTTONS];
    int mousex = 0;
    int mousey = 0;
    int mousex_prev = 0;
    int mousey_prev = 0;
};
extern Input* input;
#endif // INPUT_H
