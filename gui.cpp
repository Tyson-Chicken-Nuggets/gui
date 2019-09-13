#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include <nanogui/colorpicker.h>
#include <PerlinNoise.hpp>
#include <iostream>
#include <type_traits>
#include <string>
#include <cstdlib>

#include <cstdint>
#include <memory>
#include <utility>
#include <bmp.hpp>

#if defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
#if defined(_WIN32)
    #pragma warning(push)
    #pragma warning(disable: 4457 4456 4005 4312)
#endif

/* #define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> */

class ExampleApplication : public nanogui::Screen{
    public:
        ExampleApplication() : nanogui::Screen(Eigen::Vector2i(500, 500), "Color Button"){
            using namespace nanogui;

            //code goes here
            //check testgui.cpp for examples

            performLayout();
            
        }
};

int main(){
    return 0;
}