#ifndef GUICLASS_H 
#define GUICLASS_H


#include <SFML/Graphics.hpp>
#include <random>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <string>
#include <complex>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

#include "GUIClass.h"
#include "WaveClasses.h"


class ManageGUI{
private:
    tgui::EditBox::Ptr editBox1, editBox2, editBox3, editBox4;

    tgui::Label::Ptr  label1, label2, label3, label4;
    tgui::CheckBox::Ptr checkbox;
    tgui::Button::Ptr button;

    tgui::Gui *gui_to_manage;
        
    int wl,sources_width,n_sources;
    int wave_speed;
    
    bool was_button_pressed;
    int wave_form;
public:
    ManageGUI(tgui::Gui &input_gui, GridWave &input_wg);
        
    std::vector<int> getNewParameters(sf::RenderWindow &input_window);
   
    void buttonTextBack(sf::RenderWindow &input_window);
    
};

#endif // GUICLASS_H 
