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
    tgui::EditBox::Ptr editBox_WL, editBox_nsources, editBox_wave_width, editBox_speed,editBox_slits_distance;

    tgui::Label::Ptr  label_WL, label_nsources, label_wave_width, label_speed, label_slits_distance;
    tgui::CheckBox::Ptr checkbox_amplitude,checkbox_power;
    tgui::Button::Ptr button;
    tgui::RadioButton::Ptr radioButton_plane, radioButton_spherical, radioButton_doubleSlit;
    tgui::Gui *gui_to_manage;
    
    tgui::Label::Ptr label_option, label_parameters;
    

    bool was_button_pressed;
    
    unsigned int last_RB_option;
    unsigned int initial_submit_yposition;
    
    GFParameters gf_params;
    
public:
    ManageGUI( tgui::Gui &input_gui, GFParameters input_gf_params);
        
    GFParameters getNewParameters(sf::RenderWindow &input_window);
   
    void buttonTextBack();

    void setGUIGeneral();
    void updateGUIifChecked();
    
    void setSubmitWidgetCoord(unsigned int position_opt_submit_offset);
    void setGeneralWidgets(unsigned int position_offset);
    //void setContextualWidgets(unsigned int position_offset);
    
    void setPlaneWidgets();
    void setSphericalWidgets();
    void setDoubleSlitWidgets();
    
    void updateContextualWidgets();
    
    void setAllWidgets();

    void createAllWidget();
    void showErrorWindow(std::string errorMessage);

};

#endif // GUICLASS_H 
