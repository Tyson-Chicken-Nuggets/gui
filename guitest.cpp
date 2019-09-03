#include <nanogui/nanogui.h>
#include <cstdlib>
#include <iostream>
using namespace nanogui;

void color(int* c){
    
    srand(time(NULL));
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;

    c[0] = r;
    c[1] = g;
    c[2] = b;
}

void setWinColor(){

}

int main(int, char **){
    
    //initiate gui
    nanogui::init();

    //create random color
    int rgb[3];
    color(&(*rgb));
    Color background(rgb[0], rgb[1], rgb[2], 255);

    //creat screen instance
    Screen *screen = nullptr;
    screen = new Screen(Vector2i(1000, 1000), "Color Button");

    //add color changer window
    FormHelper *gui = new FormHelper(screen);
    ref<Window> window = gui->addWindow(Eigen::Vector2i(10,10), "Window Color");
    gui->addVariable("Color", background)
       ->setFinalCallback([](const Color &c){
           std::cout <<  "ColorPicker Final Callback: ["
                     << c.r() << ", "
                     << c.g() << ", "
                     << c.b() << ", "
                     << c.w() << "]" << std::endl;
         });
    gui->addButton("Change Color", []() {std::cout << "Button pressed." << std::endl;});

}