#ifndef KEY_EVENT_H
#define KEY_EVENT_H

struct KeyEvent
{
    int key;
    int scancode;
    int action;
    int mods;
};

#endif