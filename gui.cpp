#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/colorwheel.h>
#include <nanogui/colorpicker.h>
#include <iostream>
#include <string>
#include <cstdlib>



class ExampleApplication : public nanogui::Screen{
    private:

        Screen* win = this;
        int rgb[3];

        void color(int* c){
    
            srand(time(NULL));
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;

            c[0] = r;
            c[1] = g;
            c[2] = b;
        }

        void winColor(nanogui::Screen* win, nanogui::Color background){
            win->setBackground(background);
            std::cout <<  "New Screen Color: ["
                      << background.r() << ", "
                      << background.g() << ", "
                      << background.b() << "]" << std::endl; 
        }

    public:
        ExampleApplication() : nanogui::Screen(Eigen::Vector2i(1000, 1000), "Color Button"){
            using namespace nanogui;

            //create window
            Window *window = new Window(this, "Windwow Color");
            window->setPosition(Vector2i(0,0));
            GridLayout *layout = new GridLayout(Orientation::Horizontal, 2, Alignment::Middle, 15, 5);
            layout->setColAlignment({Alignment::Maximum, Alignment::Fill});
            layout->setSpacing(0, 10);
            window->setLayout(layout);

            //create color picker widget
            new Label(window, "Background Color:", "sans-bold");
            color(&(*rgb));
            auto cp = new ColorPicker(window, {rgb[0], rgb[1], rgb[2], 255});
            cp->setFixedSize({100, 20});
            if(cp->pushed){
                winColor(win, cp->color);
            }
            cp->setFinalCallback([](const Color &c){
                std::cout <<  "ColorPicker Final Callback: ["
                          << c.r() << ", "
                          << c.g() << ", "
                          << c.b() << "]" << std::endl;
            });

            //create style button widget
            new Label(window, "Random Color:", "sans-bold");
            Button *b = window->add<Button>("Styled");
            b->setCallback([]{std::cout << "Changed Color" << std::endl;});
            if (b->pushed()){
                color(&(*rgb));
                winColor(win, Color(rgb[0], rgb[1], rgb[2], 255));
                b->setBackgroundColor(Color(rgb[0], rgb[1], rgb[2], 255));
                cp->setColor(Color(rgb[0], rgb[1], rgb[2], 255));
            }
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