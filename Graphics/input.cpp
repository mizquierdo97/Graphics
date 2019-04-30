#include "input.h"
#include <QMouseEvent>
#include <QKeyEvent>

Input::Input()
{
    input = this;
}

void Input::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() >= 0) && (event->key() <= MAX_KEYS))
          keys[event->key()] = KeyState::Down;
}

void Input::keyReleaseEvent(QKeyEvent *event)
{
    if ((event->key() >= 0) && (event->key() <= MAX_KEYS))
          keys[event->key()] = KeyState::Up;
}

void Input::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() >= 0) && (event->button() <= MAX_BUTTONS))
        mouseButtons[event->button()] = MouseButtonState::Down;
}

void Input::mouseMoveEvent(QMouseEvent *event)
{
    mousex_prev = event->x() - mousex;
    mousey_prev = event->y() - mousey;
    mousex = event->x();
    mousey = event->y();
}

void Input::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() >= 0) && (event->button() <= MAX_BUTTONS))
        mouseButtons[event->button()] = MouseButtonState::Up;
}
