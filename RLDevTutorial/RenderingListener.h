#pragma once
#include "Listener.h"

#include "Globals.h"
#include "Level.h"

class RenderingListener :
    public Listener
{
    private:
    int FireRenderEvent(RenderEvent *e);

    public:
    RenderingListener();
};

