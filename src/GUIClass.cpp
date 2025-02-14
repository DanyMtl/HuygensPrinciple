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
#include "textOutput.h"


#define FONT_SIZE 13

//********************************************************************
// This ManageGUI constructor calls all the member functions to
// set up the user input window and set up all the necessary variables.
//********************************************************************
ManageGUI::ManageGUI( tgui::Gui &input_gui, GFParameters input_gf_params){
    
    gf_params=input_gf_params;
    
    textOutput::setLanguage("fr");
    
    gui_to_manage=&input_gui;

    
    setGeneralWidgets(10);
    updateContextualWidgets();
    
    was_button_pressed=false;
        
    button->onPress([&](){
        std::string userInput;

        userInput=editBox_WL->getText().toStdString();
        if(!userInput.empty()) gf_params.wavelength = std::stoi(userInput);
        userInput=editBox_nsources->getText().toStdString();
        if(!userInput.empty()) gf_params.n_sources = std::stoi(userInput);
        userInput=editBox_wave_width->getText().toStdString();
        if(!userInput.empty()) gf_params.slit_width = std::stoi(userInput);
        userInput=editBox_speed->getText().toStdString();
        if(!userInput.empty()) gf_params.speed = std::stoi(userInput);
        userInput=editBox_slits_distance->getText().toStdString();
        
        if (checkbox_amplitude->isChecked())
            gf_params.amplitude_func =amplitudeForm::costheta;
        else
            gf_params.amplitude_func =amplitudeForm::one;
        
        if (checkbox_power->isChecked())
            gf_params.show_intensity =true;
        else
            gf_params.show_intensity =false;
        

        
        
        
        if(!userInput.empty()) gf_params.slits_distance = std::stoi(userInput);
        
        if (radioButton_plane->isChecked()){
            gf_params.initial_wave_form =waveForm::plane;
        }
        else if (radioButton_spherical->isChecked()){
            gf_params.initial_wave_form =waveForm::spherical;
        }
        else if (radioButton_doubleSlit->isChecked()){
            gf_params.initial_wave_form =waveForm::doubleSlit;
        }
        else{
            std::cerr << "Initial wave form not valid.\nThe problem is in the ManageGUI Class." <<std::endl;
        }
        was_button_pressed=true;
    });
        
}


//********************************************************************
// The member function setSubmitWidgetCoord(...) sets the y coordinate
// of the submit button.
//********************************************************************

void ManageGUI::setSubmitWidgetCoord(unsigned int position_offset){

    button->setPosition(100, position_offset);
    
    
}

//********************************************************************
// The member function createAllWidget(...) creates all the widgets
// necessary for the GUI.
//********************************************************************

void ManageGUI::createAllWidget()
{
    
}

//********************************************************************
// The member function setPlaneWidgets(...)  sets up the widgets
// configuration and text for the "Plane wave" option.
//********************************************************************

void ManageGUI::setPlaneWidgets(){
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);
    label_wave_width->setText(textOutput::getText("label_wave_width_plane"));
    label_nsources->setText(textOutput::getText("label_nsources"));

    return;
}
//********************************************************************
// The member function setSphericalWidgets(...)  sets up the widgets
// configuration and text for the "Spherical wave" option.
//********************************************************************

void ManageGUI::setSphericalWidgets(){
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);
    label_wave_width->setText(textOutput::getText("label_wave_width_spherical"));
    label_nsources->setText(textOutput::getText("label_nsources"));
    return;
}


//********************************************************************
// The member function setPlaneWidgets(...)  sets up the widgets
// configuration and text for the "Double slit" option.
//********************************************************************

void ManageGUI::setDoubleSlitWidgets(){
    
    label_wave_width->setText(textOutput::getText("label_wave_width_doubleSlit"));
    label_nsources->setText(textOutput::getText("label_nsources_doubleSlit"));
    
    gui_to_manage->add(editBox_slits_distance);
    gui_to_manage->add(label_slits_distance);
}

//********************************************************************
// The member function updateContextualWidgets(...)  manages the
// contextual widgets, those that depend on the options selected by the user.
//********************************************************************

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
        setSubmitWidgetCoord(initial_submit_yposition);
           
    }
    else if(current_RB_option==waveForm::spherical){

        setSphericalWidgets();
        setSubmitWidgetCoord(initial_submit_yposition);
    }
    else if(current_RB_option==waveForm::doubleSlit){

        setDoubleSlitWidgets();
        setSubmitWidgetCoord(initial_submit_yposition+35);
    }
    
    
    last_RB_option=current_RB_option;
    return;
}


//********************************************************************
// The member function setGeneralWidgets(...) sets up all the
// widgets in the input windows and will make available all the general ones.
//********************************************************************

void ManageGUI::setGeneralWidgets(unsigned int position_offset){
    
    unsigned int widget_y_position=position_offset;
    last_RB_option=999;
    
    label_option=tgui::Label::create(textOutput::getText("label_option"));
    label_option->setPosition(5,widget_y_position);
    label_option->setTextSize(16);
    gui_to_manage->add(label_option);
    
    widget_y_position+=20;
    radioButton_plane=tgui::RadioButton::create();
    radioButton_plane->setPosition(20, widget_y_position);
    radioButton_plane->setText(textOutput::getText("radioButton_plane"));
    radioButton_plane->setTextClickable(true);
    radioButton_plane->setChecked(true);
    gui_to_manage->add(radioButton_plane);
    
    widget_y_position+=22;
    radioButton_spherical=tgui::RadioButton::create();
    radioButton_spherical->setPosition(20,widget_y_position);
    radioButton_spherical->setText(textOutput::getText("radioButton_spherical"));
    radioButton_spherical->setTextClickable(true);
    gui_to_manage->add(radioButton_spherical);

    widget_y_position+=22;
    radioButton_doubleSlit=tgui::RadioButton::create();
    radioButton_doubleSlit->setPosition(20, widget_y_position);
    radioButton_doubleSlit->setText(textOutput::getText("radioButton_doubleSlit"));
    radioButton_doubleSlit->setTextClickable(true);
    gui_to_manage->add(radioButton_doubleSlit);
    

 
    widget_y_position+=31;
    label_parameters=tgui::Label::create(textOutput::getText("label_parameters"));
    label_parameters->setPosition(5,widget_y_position);
    label_parameters->setTextSize(16);
    gui_to_manage->add(label_parameters);
    
    widget_y_position+=23;
    checkbox_amplitude = tgui::CheckBox::create(textOutput::getText("checkbox_amplitude"));
    checkbox_amplitude->setPosition(20, widget_y_position);
    if(gf_params.amplitude_func==amplitudeForm::costheta)
        checkbox_amplitude->setChecked(true);
    gui_to_manage->add(checkbox_amplitude);
  
    widget_y_position+=23;
    checkbox_power = tgui::CheckBox::create(textOutput::getText("checkbox_power"));
    checkbox_power->setPosition(20, widget_y_position);
    if(gf_params.show_intensity==true)
        checkbox_power->setChecked(true);
    gui_to_manage->add(checkbox_power);
    
    widget_y_position+=22;
    editBox_WL = tgui::EditBox::create();
    editBox_WL->setSize(100, 30);
    editBox_WL->setPosition(10, widget_y_position);
    editBox_WL->setText(std::to_string(gf_params.wavelength));
    editBox_WL->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_WL);
    
    label_WL = tgui::Label::create(textOutput::getText("label_WL"));
    label_WL->setPosition(120, widget_y_position+7);
    label_WL->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_WL);

    widget_y_position+=35;
    editBox_speed = tgui::EditBox::create();
    editBox_speed->setSize(100, 30);
    editBox_speed->setPosition(10,widget_y_position);
    editBox_speed->setText(std::to_string(gf_params.speed));
    editBox_speed->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_speed);

    label_speed= tgui::Label::create(textOutput::getText("label_speed"));
    label_speed->setPosition(120, widget_y_position);
    label_speed->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_speed);
    
    widget_y_position+=35;
    editBox_nsources = tgui::EditBox::create();
    editBox_nsources->setSize(100, 30);
    editBox_nsources->setPosition(10, widget_y_position);
    editBox_nsources->setText(std::to_string(gf_params.n_sources));
    editBox_nsources->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_nsources);

    label_nsources = tgui::Label::create(textOutput::getText("label_nsources"));
    label_nsources->setPosition(120, widget_y_position+6);
    label_nsources->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_nsources);
    
    
    widget_y_position+=35;
    editBox_wave_width = tgui::EditBox::create();
    editBox_wave_width->setSize(100, 30);
    editBox_wave_width->setPosition(10, widget_y_position);
    editBox_wave_width->setText(std::to_string(gf_params.slit_width));
    editBox_wave_width->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_wave_width);

    label_wave_width= tgui::Label::create(textOutput::getText("label_wave_width_plane"));
    label_wave_width->setPosition(120, widget_y_position);
    label_wave_width->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_wave_width);
    
    initial_submit_yposition=widget_y_position + editBox_wave_width->getSize().y+15;

    
    widget_y_position+=35;
    editBox_slits_distance=tgui::EditBox::create();
    editBox_slits_distance->setSize(100, 30);
    editBox_slits_distance->setPosition(10, widget_y_position);
    editBox_slits_distance->setText(std::to_string(gf_params.slits_distance));
    editBox_slits_distance->setInputValidator(tgui::EditBox::Validator::UInt);
    
    label_slits_distance=tgui::Label::create(textOutput::getText("label_slits_distance"));
    label_slits_distance->setPosition(120,widget_y_position+6);
    label_slits_distance->setTextSize(FONT_SIZE);


    
    button = tgui::Button::create(textOutput::getText("submit_button"));
    button->setSize(120, 40);
    
    
    setSubmitWidgetCoord(initial_submit_yposition);
    
    gui_to_manage->add(button);



}

// *******************************************************************
// The member function getNewParameters(...) outputs a vector containing
// the values entered in the user input window. In order, the vector
// will contain:
//   0: the wavelenght  1: the number of sources 2: the width of the slit
//   3: the speed of the wave  4: the form of the initial wave
//   5: the distance between the slits  6: the k dependency of the amplitude
// *******************************************************************

GFParameters ManageGUI::getNewParameters(sf::RenderWindow &input_window)
{
    if(gf_params.initial_wave_form==waveForm::doubleSlit){
        if(gf_params.slit_width>=gf_params.slits_distance){
            showErrorWindow(textOutput::getText("error_doubleSlit"));
            gf_params.initial_wave_form=waveForm::plane;
            gf_params.slit_width*=2;
            radioButton_plane->setChecked(true);
            editBox_wave_width->setText(std::to_string(gf_params.slit_width));
        }
        
    }
    
    if(was_button_pressed)
    {
        button->setText(textOutput::getText("submit_button_calc"));
        input_window.clear(sf::Color(255,255,255));
        gui_to_manage->draw();
        input_window.display();
        
        was_button_pressed=false;
        return gf_params;
    }
    else
        return GFParameters() ;
    
}
void ManageGUI::buttonTextBack()
{
    button->setText(textOutput::getText("submit_button"));
    gui_to_manage->draw();


}
// *******************************************************************
// The member function showErrorWindow(...) creates a window with an
// error message in it.
// *******************************************************************

void ManageGUI::showErrorWindow(std::string errorMessage) {
    sf::RenderWindow errorWindow(sf::VideoMode({300, 200}), "Erreur", sf::Style::Titlebar | sf::Style::Close);
    tgui::Gui gui(errorWindow);
    
    auto label_err_msg=tgui::Label::create(errorMessage);
    
    label_err_msg->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    label_err_msg->setVerticalAlignment(tgui::VerticalAlignment::Center);
    label_err_msg->setTextSize(15);
    
    gui.add(label_err_msg);
    
    tgui::Layout2d windowSize = gui.getView().getSize();
    unsigned int label_x_pos=(gui.getView().getSize().x - label_err_msg->getSize().x)/2;
    unsigned int label_y_pos=(gui.getView().getSize().y - label_err_msg->getSize().y)/2-30;
   
    label_err_msg->setPosition(label_x_pos,label_y_pos);
    
    
    auto ok_button = tgui::Button::create("Ok");
    ok_button->setSize(60, 40);

    unsigned int button_x_pos=(gui.getView().getSize().x - ok_button->getSize().x)/2;
    unsigned int button_y_pos=label_y_pos+label_err_msg->getSize().y+30;


    ok_button->setPosition(button_x_pos, button_y_pos);
    gui.add(ok_button);

    ok_button->onPress([&](){
        errorWindow.close();
    });
    
    
    while (errorWindow.isOpen()) {
        while (const std::optional event = errorWindow.pollEvent()){
            // "close requested" event: we close the mainWindow
            if (event->is<sf::Event::Closed>())
            {
                errorWindow.close();
            }
            gui.handleEvent(*event);
        }

        errorWindow.clear(sf::Color(200,200,200));
        gui.draw();
        errorWindow.display();
    }
    
}

//void ManageGUI::updateGUIifChecked()
//{
//    button->setText("Soumettre");
//    gui_to_manage->draw();


//}

