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

#define FONT_SIZE 13


ManageGUI::ManageGUI(tgui::Gui &input_gui, GridWave &input_wg){
        

    gui_to_manage=&input_gui;
    wl=input_wg.getWaveLength();
    wave_width=input_wg.getSourcesWidth();
    n_sources=input_wg.getNSources();
    wave_speed= (float)wl/input_wg.getWavePeriod();
    slits_distance = input_wg.getSlitsDistance();
    amplitude_form= input_wg.getAmplitudeForm();


    createAllWidget();
    
    setGeneralWidgets(10);
    updateContextualWidgets();
    
    was_button_pressed=false;
        
    button->onPress([&](){
        //int userValue;
        std::string userInput;

        userInput=editBox_WL->getText().toStdString();
        if(!userInput.empty()) wl = std::stoi(userInput);
        userInput=editBox_nsources->getText().toStdString();
        if(!userInput.empty()) n_sources = std::stoi(userInput);
        userInput=editBox_wave_width->getText().toStdString();
        if(!userInput.empty()) wave_width = std::stoi(userInput);
        userInput=editBox_speed->getText().toStdString();
        if(!userInput.empty()) wave_speed = std::stoi(userInput);
        userInput=editBox_slits_distance->getText().toStdString();
        
        if (checkbox_amplitude->isChecked()){
            amplitude_form=amplitudeForm::costheta;
        }
        else{
            amplitude_form=amplitudeForm::one;
        }
        
        
        if(!userInput.empty()) slits_distance = std::stoi(userInput);
        
        if (radioButton_plane->isChecked()){
            wave_form=waveForm::plane;
        }
        else if (radioButton_spherical->isChecked()){
            wave_form=waveForm::spherical;
        }
        else if (radioButton_doubleSlit->isChecked()){
            wave_form=waveForm::doubleSlit;
        }
        else{
            std::cerr << "Initial wave form not valid.\nThe problem is in the ManageGUI Class." <<std::endl;
        }
        was_button_pressed=true;
    });
        
}

void ManageGUI::setSubmitWidgetCoord(unsigned int position_offset){

    button->setSize(120, 40);
    button->setPosition(100, position_offset);
    
    
}

void ManageGUI::createAllWidget()
{
    radioButton_plane=tgui::RadioButton::create();
    radioButton_spherical=tgui::RadioButton::create();
    radioButton_doubleSlit=tgui::RadioButton::create();
    
    radioButton_plane->setChecked(true);
    
    checkbox_amplitude = tgui::CheckBox::create("Dépendance en θ de l'amplitude");

    
    last_RB_option=999;
    
    editBox_WL = tgui::EditBox::create();
    label_WL = tgui::Label::create("Longueur d'onde (en pixels)");
    editBox_speed = tgui::EditBox::create();
    label_speed= tgui::Label::create("Vitesse de l'onde\n(pixels/seconde)");
    editBox_nsources = tgui::EditBox::create();
    label_nsources = tgui::Label::create("Nombre de sources");
    editBox_wave_width = tgui::EditBox::create();
    label_wave_width= tgui::Label::create("Largeur de l'onde plane plane\ninitiale (en pixels)");
    button = tgui::Button::create("Soumettre");
    
    label_slits_distance=tgui::Label::create("Distance entre les fentes\n(centre à centre)");
    editBox_slits_distance=tgui::EditBox::create();
    
    label_option=tgui::Label::create("Choisir la forme de l'onde initiale:");
    label_parameters=tgui::Label::create("Définir les paramètres:");

}

void ManageGUI::setPlaneWidgets(){
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);
    label_wave_width->setText("Largeur de l'onde plane\ninitiale (en pixels)");
    label_nsources->setText("Nombre de sources");

    return;
}

void ManageGUI::setSphericalWidgets(){
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);
    label_wave_width->setText("Circonférence de l'onde\nsphérique initiale (en pixels)");
    label_nsources->setText("Nombre de sources");
    return;
}

void ManageGUI::setDoubleSlitWidgets(unsigned int position_offset){
    editBox_slits_distance->setPosition(10, position_offset);
    label_slits_distance->setPosition(120,position_offset+6);
    

    
    label_wave_width->setText("Largeur de chaque fente\n(en pixels)");
    label_nsources->setText("Nombre de sources par fente");
    
    gui_to_manage->add(editBox_slits_distance);
    gui_to_manage->add(label_slits_distance);
}

void ManageGUI::updateContextualWidgets(){
    unsigned int current_RB_option=9999;
    if (radioButton_plane->isChecked()){
        current_RB_option=waveForm::plane;
    }
    else if (radioButton_spherical->isChecked()){
        current_RB_option=waveForm::spherical;
    }
    else if (radioButton_doubleSlit->isChecked()){
        current_RB_option=waveForm::doubleSlit;
    }
    else
    {
        std::cerr << "Wave type selected is not defined.\nCheck ManageGUI::setContextualWidgets() for the error." << std::endl;
        exit(0);
    }
    
    if(current_RB_option==last_RB_option)
        return;
 
    if(current_RB_option==waveForm::plane){
       
        setPlaneWidgets();
        setSubmitWidgetCoord(310);
           
    }
    else if(current_RB_option==waveForm::spherical){

        setSphericalWidgets();
        setSubmitWidgetCoord(310);
    }
    else if(current_RB_option==waveForm::doubleSlit){

        setDoubleSlitWidgets(290);
        setSubmitWidgetCoord(340);
    }
    
    
    last_RB_option=current_RB_option;
    return;
}


void ManageGUI::setGeneralWidgets(unsigned int position_offset){
    
    label_option->setPosition(5,position_offset+0);
    label_option->setTextSize(16);
    gui_to_manage->add(label_option);
    
    radioButton_plane->setPosition(20, position_offset + 20);
    radioButton_plane->setText("Onde plane (1 fente)");
    radioButton_plane->setTextClickable(true);
    gui_to_manage->add(radioButton_plane);
    
                       
    radioButton_spherical->setPosition(20,position_offset+42);
    radioButton_spherical->setText("Onde sphérique");
    radioButton_spherical->setTextClickable(true);
    gui_to_manage->add(radioButton_spherical);

    radioButton_doubleSlit->setPosition(20, position_offset+64);
    radioButton_doubleSlit->setText("Deux fentes");
    radioButton_doubleSlit->setTextClickable(true);
    gui_to_manage->add(radioButton_doubleSlit);
    

 
    
    label_parameters->setPosition(5,position_offset+95);
    label_parameters->setTextSize(16);
    gui_to_manage->add(label_parameters);
    
    checkbox_amplitude->setPosition(20, position_offset+118);
    if(amplitude_form==amplitudeForm::costheta)
        checkbox_amplitude->setChecked(true);
    gui_to_manage->add(checkbox_amplitude);
  
    
    editBox_WL->setSize(100, 30);
    editBox_WL->setPosition(10, position_offset+140);
    editBox_WL->setDefaultText(std::to_string(wl));
    editBox_WL->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_WL);
    
    label_WL->setPosition(120, position_offset+148);
    label_WL->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_WL);

    editBox_speed->setSize(100, 30);
    editBox_speed->setPosition(10,position_offset+175);
    editBox_speed->setDefaultText(std::to_string(wave_speed));
    editBox_speed->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_speed);

    label_speed->setPosition(120, position_offset+175);
    label_speed->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_speed);
    
    editBox_nsources->setSize(100, 30);
    editBox_nsources->setPosition(10, position_offset+210);
    editBox_nsources->setDefaultText(std::to_string(n_sources));
    editBox_nsources->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_nsources);


    label_nsources->setPosition(120, position_offset+217);
    label_nsources->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_nsources);
    
    
    editBox_wave_width->setSize(100, 30);
    editBox_wave_width->setPosition(10, position_offset+245);
    editBox_wave_width->setDefaultText(std::to_string(wave_width));
    editBox_wave_width->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_wave_width);

    label_wave_width->setPosition(120, position_offset+245);
    label_wave_width->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_wave_width);
    

    label_slits_distance->setTextSize(FONT_SIZE);
    editBox_slits_distance->setSize(100, 30);
    editBox_slits_distance->setDefaultText(std::to_string(slits_distance));
    editBox_slits_distance->setInputValidator(tgui::EditBox::Validator::UInt);

    
    setSubmitWidgetCoord(300);
    gui_to_manage->add(button);



}

           

std::vector<int> ManageGUI::getNewParameters(sf::RenderWindow &input_window)
{

    
    if(was_button_pressed)
    {
        button->setText("Calculs en cours");
        input_window.clear(sf::Color(255,255,255));
        gui_to_manage->draw();
        input_window.display();
        
        was_button_pressed=false;
        return {wl,n_sources,wave_width,wave_speed,wave_form,slits_distance,(int)amplitude_form};
    }
    else
        return {};
    
}
void ManageGUI::buttonTextBack()
{
    button->setText("Soumettre");
    gui_to_manage->draw();


}

//void ManageGUI::updateGUIifChecked()
//{
//    button->setText("Soumettre");
//    gui_to_manage->draw();


//}

