//
// Created by mtesseract on 07-02-20.
//

#ifndef VALCANO_RENDERER_HPP
#define VALCANO_RENDERER_HPP

#include <gfx/window/ContextWindow.hpp>

namespace mt::gfx {
    class Renderer{
    protected:
        std::shared_ptr<ContextWindow> window;

    public:
        explicit Renderer(std::shared_ptr<ContextWindow> window) : window(window){};

        virtual void draw() = 0;

        virtual void terminate() = 0;
    };
}

#endif //VALCANO_RENDERER_HPP
