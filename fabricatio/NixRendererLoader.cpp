#include <dlfcn.h>
#include <iostream>

#include "NixRendererLoader.h"

Renderer* (*create)();
void (*destroy)(Renderer*);

Renderer* createRenderer()
{
    void* handle = dlopen("producentis.so", RTLD_LAZY);
    std::cout << "Renderer loaded" << std::endl;
    create = (Renderer* (*)())dlsym(handle, "CreateRenderer");
    destroy = (void (*)(Renderer*))dlsym(handle, "DestroyRenderer");
    Renderer* renderer = create();
    return renderer;
}

void destroyRenderer(Renderer* renderer)
{
    destroy(renderer);
}

