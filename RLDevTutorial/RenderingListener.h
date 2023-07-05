#pragma once
#include "Listener.h"
class RenderingListener :
    public Listener
{
    private:
    int FireRenderEvent(RenderEvent *e);

    public:
    RenderingListener();
};

