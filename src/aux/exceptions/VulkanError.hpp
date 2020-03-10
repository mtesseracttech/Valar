//
// Created by mtesseract on 01-01-20.
//

#include <stdexcept>
#include <string>

#ifndef VALCANO_VULKANERROR_HPP
#define VALCANO_VULKANERROR_HPP

#endif // VALCANO_VULKANERROR_HPP

namespace mt::aux {
class VulkanError : public std::runtime_error {
private:
    std::string m_text;

public:
    explicit VulkanError(const char* p_message)
        : std::runtime_error("Vulkan Error")
    {
        m_text = p_message;
    };

    VulkanError()
        : VulkanError("Vulkan Error")
    {
    }

    virtual const char* what() const throw() { return m_text.c_str(); }
};
} // namespace mt::aux