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


    slider_scale->onValueChange([&](FLOAT slider_value){
        gf_params.view_scale_factor=slider_value;
    });

    checkbox_intensity->onChange([this]() {

        if(checkbox_intensity->isChecked()){
            gf_params.show_intensity=true;
        }
        else{
            gf_params.show_intensity=false;
        }
        


    });
    
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
        
        if (checkbox_intensity->isChecked())
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
    
    comboBox_language->onItemSelect([&](int index){
        if(index==0)
        {
            textOutput::setLanguage("en");
        }
        else if(index==1)
        {
            textOutput::setLanguage("fr");
        }
        setTextInGUI();
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
    
    gui_to_manage->add(editBox_slits_distance,  "EditBoxSlitsDistance");
    gui_to_manage->add(label_slits_distance, "LabelSlitsDistance");
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
    
    label_option=tgui::Label::create();
    label_option->setPosition(5,widget_y_position);
    label_option->setTextSize(FONT_SIZE+3);
    gui_to_manage->add(label_option, "OptionWaveForm");
    
    widget_y_position+=25;
    radioButton_plane=tgui::RadioButton::create();
    radioButton_plane->setPosition(20, widget_y_position);
    radioButton_plane->setTextClickable(true);
    radioButton_plane->setChecked(true);
    gui_to_manage->add(radioButton_plane, "RadioButtonPlane");
    
    widget_y_position+=22;
    radioButton_spherical=tgui::RadioButton::create();
    radioButton_spherical->setPosition(20,widget_y_position);
    radioButton_spherical->setTextClickable(true);
    gui_to_manage->add(radioButton_spherical, "RadioButtonSphere");

    widget_y_position+=22;
    radioButton_doubleSlit=tgui::RadioButton::create();
    radioButton_doubleSlit->setPosition(20, widget_y_position);
    radioButton_doubleSlit->setTextClickable(true);
    gui_to_manage->add(radioButton_doubleSlit, "RadioButtonDoubleSlit");
    
    widget_y_position+=30;
    separator1 = tgui::SeparatorLine::create();
    separator1->setPosition({0, widget_y_position});
    separator1->setSize({500, 2});
    separator1->getRenderer()->setColor(tgui::Color::Black);
    gui_to_manage->add(separator1,"separator1");

 
    widget_y_position+=15;
    label_parameters=tgui::Label::create();
    label_parameters->setPosition(5,widget_y_position);
    label_parameters->setTextSize(FONT_SIZE+3);
    gui_to_manage->add(label_parameters, "LabelParameters");
    
    widget_y_position+=28;
    checkbox_amplitude = tgui::CheckBox::create();
    checkbox_amplitude->setPosition(20, widget_y_position);
    if(gf_params.amplitude_func==amplitudeForm::costheta)
        checkbox_amplitude->setChecked(true);
    gui_to_manage->add(checkbox_amplitude, "CheckBoxAmplitude");
  
    
    widget_y_position+=30;
    editBox_WL = tgui::EditBox::create();
    editBox_WL->setSize(100, 30);
    editBox_WL->setPosition(10, widget_y_position);
    editBox_WL->setText(std::to_string(gf_params.wavelength));
    editBox_WL->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_WL, "EditBoxWavelength");
    
    label_WL = tgui::Label::create();
    label_WL->setPosition(120, widget_y_position+7);
    label_WL->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_WL, "LabelWavelenght");

    widget_y_position+=35;
    editBox_speed = tgui::EditBox::create();
    editBox_speed->setSize(100, 30);
    editBox_speed->setPosition(10,widget_y_position);
    editBox_speed->setText(std::to_string(gf_params.speed));
    editBox_speed->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_speed, "EditBoxSpeed");

    label_speed= tgui::Label::create();
    label_speed->setPosition(120, widget_y_position);
    label_speed->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_speed, "LabelSpeed");
    
    widget_y_position+=35;
    editBox_nsources = tgui::EditBox::create();
    editBox_nsources->setSize(100, 30);
    editBox_nsources->setPosition(10, widget_y_position);
    editBox_nsources->setText(std::to_string(gf_params.n_sources));
    editBox_nsources->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_nsources, "EditBoxNumberSources");

    label_nsources = tgui::Label::create();
    label_nsources->setPosition(120, widget_y_position+6);
    label_nsources->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_nsources, "LabelNumberSources");
    
    
    widget_y_position+=35;
    editBox_wave_width = tgui::EditBox::create();
    editBox_wave_width->setSize(100, 30);
    editBox_wave_width->setPosition(10, widget_y_position);
    editBox_wave_width->setText(std::to_string(gf_params.slit_width));
    editBox_wave_width->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox_wave_width, "EditBoxWaveWidth");

    label_wave_width= tgui::Label::create();
    label_wave_width->setPosition(120, widget_y_position);
    label_wave_width->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_wave_width, "LabelWaveWidth");
    
    initial_submit_yposition=widget_y_position + editBox_wave_width->getSize().y+15;

    
    widget_y_position+=35;
    editBox_slits_distance=tgui::EditBox::create();
    editBox_slits_distance->setSize(100, 30);
    editBox_slits_distance->setPosition(10, widget_y_position);
    editBox_slits_distance->setText(std::to_string(gf_params.slits_distance));
    editBox_slits_distance->setInputValidator(tgui::EditBox::Validator::UInt);
    
    label_slits_distance=tgui::Label::create();
    label_slits_distance->setPosition(120,widget_y_position+6);
    label_slits_distance->setTextSize(FONT_SIZE);


    widget_y_position+=70;
    button = tgui::Button::create();
    button->setSize(120, 40);
    setSubmitWidgetCoord(initial_submit_yposition);
    gui_to_manage->add(button, "SubmitButton");
    button->setFocused(true);

    widget_y_position+=40;
    
    separator2 = tgui::SeparatorLine::create();
    separator2->setPosition({0, widget_y_position});
    separator2->setSize({500, 2});
    separator2->getRenderer()->setColor(tgui::Color::Black);
    gui_to_manage->add(separator2,"separator2");
    
    widget_y_position+=10;
    label_display = tgui::Label::create();
    label_display->setPosition(10,widget_y_position);
    label_display->setTextSize(FONT_SIZE+3);
    gui_to_manage->add(label_display,"LabelDisplay");
    
    widget_y_position+=35;
    checkbox_intensity = tgui::CheckBox::create();
    checkbox_intensity->setPosition(15, widget_y_position);
    if(gf_params.show_intensity==true)
        checkbox_intensity->setChecked(true);
    gui_to_manage->add(checkbox_intensity, "CheckBoxIntensity");

    widget_y_position+=35;
    label_slider_scale = tgui::Label::create();
    label_slider_scale->setPosition(10, widget_y_position);
    label_slider_scale->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_slider_scale, "LabelSliderScale");
    
    widget_y_position+=25;
    slider_scale=tgui::Slider::create();
    slider_scale->setMinimum(0);
    slider_scale->setMaximum(3);
    slider_scale->setStep(0.05);
    slider_scale->setValue(1);
    slider_scale->getRenderer()->setThumbColor(tgui::Color(70, 70, 70));
    slider_scale->getRenderer()->setTrackColor(tgui::Color(190, 190, 190));
    slider_scale->setPosition(30,widget_y_position);
    gui_to_manage->add(slider_scale, "SliderScale");
    
    widget_y_position+=35;
    separator3 = tgui::SeparatorLine::create();
    separator3->setPosition({0, widget_y_position});
    separator3->setSize({500, 2});
    separator3->getRenderer()->setColor(tgui::Color::Black);
    gui_to_manage->add(separator3,"separator3");
    
    widget_y_position+=10;
    label_language = tgui::Label::create();
    label_language->setPosition(10, widget_y_position);
    label_language->setTextSize(FONT_SIZE+3);
    gui_to_manage->add(label_language, "LabelSliderScale");
    
    widget_y_position+=35;
    comboBox_language=tgui::ComboBox::create();
    comboBox_language->addItem("en", "en");
    comboBox_language->addItem("fr", "fr");
    if(textOutput::getCurrentLanguage()=="fr"){
        comboBox_language->setSelectedItemByIndex(1);
    }
    if(textOutput::getCurrentLanguage()=="en"){
        comboBox_language->setSelectedItemByIndex(0);
    }
    comboBox_language->setPosition(20,widget_y_position);
    gui_to_manage->add(comboBox_language,"cmoboBox_language");
    
    setTextInGUI();
}



void ManageGUI::setTextInGUI(){
    label_option->setText(textOutput::getText("label_option"));
    radioButton_plane->setText(textOutput::getText("radioButton_plane"));
    radioButton_spherical->setText(textOutput::getText("radioButton_spherical"));
    radioButton_doubleSlit->setText(textOutput::getText("radioButton_doubleSlit"));
    label_parameters->setText(textOutput::getText("label_parameters"));
    checkbox_amplitude->setText(textOutput::getText("checkbox_amplitude"));
    label_WL->setText(textOutput::getText("label_WL"));
    label_speed->setText(textOutput::getText("label_speed"));
    label_nsources->setText(textOutput::getText("label_nsources"));
    label_wave_width->setText(textOutput::getText("label_wave_width_plane"));
    label_slits_distance->setText(textOutput::getText("label_slits_distance"));
    button->setText(textOutput::getText("submit_button"));
    label_display->setText(textOutput::getText("label_display"));
    checkbox_intensity->setText(textOutput::getText("checkbox_intensity"));
    label_slider_scale->setText(textOutput::getText("label_slider_scale"));
    label_language->setText(textOutput::getText("label_language"));

    comboBox_language->changeItemById("en",textOutput::getText("comboBox_english"));
    comboBox_language->changeItemById("fr", textOutput::getText("comboBox_french"));

   
}

// *******************************************************************
// The member function getNewParameters(...) returns the parameters
// as they were input by the user.
// *******************************************************************

GFParameters ManageGUI::getNewParameters(sf::RenderWindow &input_window)
{
    if(gf_params.initial_wave_form==waveForm::doubleSlit){
        if(gf_params.slit_width>=gf_params.slits_distance){
            showErrorWindow(textOutput::getText("error_doubleSlit"));
            gf_params.slit_width=100;
            gf_params.slits_distance=2*gf_params.slit_width;
            editBox_wave_width->setText(std::to_string( gf_params.slit_width));
            editBox_slits_distance->setText(std::to_string( gf_params.slits_distance));
           

        }
        
    }
    
    if(was_button_pressed)
    {
        button->setText(textOutput::getText("submit_button_calc"));
        input_window.clear(sf::Color(255,255,255));
        gui_to_manage->draw();
        input_window.display();
        
        was_button_pressed=false;
    }
    return gf_params;
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
    
    gui.add(label_err_msg, "ErrowWindowMessage");
    
    tgui::Layout2d windowSize = gui.getView().getSize();
    unsigned int label_x_pos=(gui.getView().getSize().x - label_err_msg->getSize().x)/2;
    unsigned int label_y_pos=(gui.getView().getSize().y - label_err_msg->getSize().y)/2-30;
   
    label_err_msg->setPosition(label_x_pos,label_y_pos);
    
    
    auto ok_button = tgui::Button::create("Ok");
    ok_button->setSize(60, 40);

    unsigned int button_x_pos=(gui.getView().getSize().x - ok_button->getSize().x)/2;
    unsigned int button_y_pos=label_y_pos+label_err_msg->getSize().y+30;


    ok_button->setPosition(button_x_pos, button_y_pos);
    gui.add(ok_button, "ErrorWindowOkButton");

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
