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

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//Add texture handling here
class Noise2D : private PerlinNoise::Perlin2D{

    public:

        #define DIM 512.0
        
        //create perlin noise from current dimensions and save to texture.bmp
        void setImage(nanogui::Screen &win, double x, double y, bool resize=false){
            
            //set seed for new noise only on button callback
            if (!resize) {seed();}

            //generate noise with frequency
            double xmin;
            double xmax;
            double ymin;
            double ymax;
            if (x > y){
                ymin = (x-y)/2 - 1;
                ymax = y - ymin;
                xmin = 0;
                xmax = x;
            } else if (y > x) {
                xmin = (y-x)/2 - 1;
                xmax = x - xmin;
                ymin = 0;
                ymax = y;
            } else {
                xmin = 0;
                xmax = x;
                ymin = 0;
                ymax = y;
            }
            double freq = 256;
            //bytes per pixel
            #define bpp 4
            double row_increment = (xmax-xmin)*bpp;
            GLubyte bmp[static_cast<int>((ymax-ymin)*row_increment)];
            for (int xm=static_cast<int>(xmin); 
                     xm<static_cast<int>(xmax); xm++){
                for (int ym=static_cast<int>(ymin); 
                         ym<static_cast<int>(ymax); ym++){

                    //define offsets
                    double xoffset = bpp * xm;
                    double yoffset = row_increment * ym;

                    //create noise
                    double weight = octaveNoise(xm / freq, ym / freq);
                    //convert noise to double between 0 and 1
                    weight += 1;
                    weight /= 2;
                    //convert weight to grayscale for rgb
                    double gray = (255*weight*0.3) + (255*weight*0.59) + (255*weight*0.11);
                    
                    //grayscale in rgba
                    bmp[static_cast<int>(xoffset + yoffset + 0)] = gray; //red
                    bmp[static_cast<int>(xoffset + yoffset + 1)] = gray; //green
                    bmp[static_cast<int>(xoffset + yoffset + 2)] = gray; //blue
                    bmp[static_cast<int>(xoffset + yoffset + 3)] = gray; //alpha
                }
            }
            std::cout << "New texture generated with seed " << static_cast<int>(Perlin2D::currentSeed()) << std::endl;
            //fix segmentatin fault (probably from array)
            glTexSubImage2D(GL_TEXTURE_2D, 0, bpp, (xmax-xmin)*bpp,
                            xmax-xmin, ymax-ymin, GL_RGBA, GL_UNSIGNED_BYTE, 
                            static_cast<const GLvoid*>(bmp));
        }

};

class ExampleApplication : public nanogui::Screen, public Noise2D{
    public:
        ExampleApplication() : nanogui::Screen(Eigen::Vector2i(500, 500), "Color Button"){
            using namespace nanogui;

            int rgb[3];
            
            //create first texture upon initialization
            setImage(*this, static_cast<double>(mFBSize[0]), 
                     static_cast<double>(mFBSize[1]));

            //create window
            Window *window = new Window(this, "Window Color");
            window->setPosition(Vector2i(0,0));
            GridLayout *layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
            layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
            layout->setSpacing(0, 10);
            window->setLayout(layout);

            //create color picker widget
            new Label(window, "Background Color:", "sans-bold");
            srand(time(NULL));
            rgb[0] = rand() % 256;
            rgb[1] = rand() % 256;
            rgb[2] = rand() % 256;
            auto cp = new ColorPicker(window, {rgb[0], rgb[1], rgb[2], 255});
            cp->setFixedSize({100, 20});
            cp->setFinalCallback([](const Color &c){
                std::cout <<  "ColorPicker Final Callback: ["
                          << c.r() << ", "
                          << c.g() << ", "
                          << c.b() << "]" << std::endl;
            });

            //create style button widget
            new Label(window, "Random Color:", "sans-bold");
            Button *b = window->add<Button>("Random Color");
            
            //create button that changes noise texture
            new Label(window, "Change Background:", "sans-bold");
            Button *tb = window->add<Button>("Random Texture");
            tb->setCallback(
                [this]{
                    setImage(*this, static_cast<double>(mFBSize[0]), 
                             static_cast<double>(mFBSize[1]));
                }
            );

            //set colors
            setBackground(nanogui::Color(rgb[0], rgb[1], rgb[2], 255));
            b->setBackgroundColor(nanogui::Color(rgb[0], rgb[1], rgb[2], 255));
            tb->setBackgroundColor(nanogui::Color(rgb[0], rgb[1], rgb[2], 255));

            //set resize callback for screen
            setResizeCallback(
                [this](const Eigen::Vector2i &s){
                    setImage(*this, static_cast<double>(s[0]), 
                             static_cast<double>(s[1]), true);
                }
            );

            //create callbacks for color changers
            b->setCallback(
                [this, b, tb, cp]{
                    const nanogui::Color col(rand()%256, rand()%256, rand()%256, 255);
                    setBackground(col);
                    b->setBackgroundColor(col);
                    tb->setBackgroundColor(col);
                    cp->setColor(col);
                    std::cout <<  "New Screen Color: ["
                              << col.r() << ", "
                              << col.g() << ", "
                              << col.b() << "]" << std::endl;

                }
            );
            cp->setFinalCallback(
                [this, b, tb, cp](const Color &c){
                    setBackground(c);
                    b->setBackgroundColor(c);
                    tb->setBackgroundColor(c);
                    cp->setColor(c);
                    std::cout <<  "New Screen Color: ["
                              << c.r() << ", "
                              << c.g() << ", "
                              << c.b() << "]" << std::endl;
                }
            );

            performLayout();
            
        }
};

int main(){
    try{
        nanogui::init();
        {
            nanogui::ref<ExampleApplication> app = new ExampleApplication();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }
        nanogui::shutdown();
    } catch (const std::runtime_error &e){
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        return -1;
    }
    return 0;
}