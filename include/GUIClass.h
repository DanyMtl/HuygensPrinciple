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
    tgui::CheckBox::Ptr checkbox_amplitude;
    tgui::Button::Ptr button;
    tgui::RadioButton::Ptr radioButton_plane, radioButton_spherical, radioButton_doubleSlit;
    tgui::Gui *gui_to_manage;
    
    tgui::Label::Ptr label_option, label_parameters;
    
    
    unsigned int amplitude_form;
    int wl,wave_width,n_sources;
    int wave_speed;
    
    int slits_distance;
    

    
    bool was_button_pressed;
    int wave_form;
    
    unsigned int last_RB_option;
public:
    ManageGUI(tgui::Gui &input_gui, GridWave &input_wg);
        
    std::vector<int> getNewParameters(sf::RenderWindow &input_window);
   
    void buttonTextBack();

    void setGUIGeneral();
    void updateGUIifChecked();
    
    void setSubmitWidgetCoord(unsigned int position_opt_submit_offset);
    void setGeneralWidgets(unsigned int position_offset);
    //void setContextualWidgets(unsigned int position_offset);
    
    void setPlaneWidgets();
    void setSphericalWidgets();
    void setDoubleSlitWidgets(unsigned int position_offset);
    
    void updateContextualWidgets();
    
    void setAllWidgets();

    void createAllWidget();
    
};

#endif // GUICLASS_H 
