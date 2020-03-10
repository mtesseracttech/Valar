//
// Created by MTesseracT on 17/02/2020.
//

#ifndef VALCANO_VERTEXBUFFER_HPP
#define VALCANO_VERTEXBUFFER_HPP

#include <vector>

#include "VertexLayout.hpp"

namespace mt::gfx {
class VertexBuffer {
};

class TriangleBuffer {
    const std::vector<Vertex> vertices = { { { 0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
        { { -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } } };
};
} // namespace mt::gfx
#endif // VALCANO_VERTEXBUFFER_HPP
