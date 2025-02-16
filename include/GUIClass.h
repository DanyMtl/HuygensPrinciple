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

    tgui::Label::Ptr  label_WL, label_nsources, label_wave_width, label_speed, label_slits_distance,label_slider_scale, label_display,label_language;
    
    tgui::SeparatorLine::Ptr separator1, separator2, separator3;
    tgui::CheckBox::Ptr checkbox_amplitude,checkbox_intensity;
    tgui::Button::Ptr button;
    tgui::RadioButton::Ptr radioButton_plane, radioButton_spherical, radioButton_doubleSlit;
    tgui::Gui *gui_to_manage;
    
    tgui::Label::Ptr label_option, label_parameters;

    tgui::Slider::Ptr slider_scale;

    tgui::ComboBox::Ptr comboBox_language;
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
    
    void setPlaneWidgets();
    void setSphericalWidgets();
    void setDoubleSlitWidgets();
    
    void updateContextualWidgets();
    
    void setAllWidgets();

    void createAllWidget();
    void showErrorWindow(std::string errorMessage);
    
    void setTextInGUI();

};

#endif // GUICLASS_H 
