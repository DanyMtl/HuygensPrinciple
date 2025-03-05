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

/*
 * Converts a real number into a string and keeps a limited number of decimals.
 *
 * number : The number to convert (float, double, etc).
 * n_decimals : The number of decimals to keep for the string.
 *
 * return : The string of the number.
 */

template <typename T> std::string keepNDecimals(T number, unsigned int n_decimals)
{
    std::ostringstream string_number;
    string_number << std::fixed << std::setprecision(n_decimals) << number;
    return string_number.str();
    
}


/*
 * This constructor prepares the widgets for the GUI. All used widget will
 * be created and will be organized. The contextual widgets are also created
 * but are not added to the GUI. The tgui::Gui object pointer will also be saved
 * to make it accessible by the Class ManageGUI.
 *
 * input_gui : The tgui::Gui object in the window that manage the GUI.
 * input_gf_params : The parameters used for the wave simulation.
 *
 */

ManageGUI::ManageGUI( tgui::Gui &input_gui, GFParameters input_gf_params){
    
    gf_params=input_gf_params;
    
    textOutput::setLanguage("fr");
    
    gui_to_manage=&input_gui;

    setGeneralWidgets();
    updateContextualWidgets();
    
    was_button_pressed=false;

    last_waveForm_option=input_gf_params.initial_wave_form;

    
    // when the user changes the value of the amplitude scale slider
    slider_scale->onValueChange([&](FLOAT slider_value){
        gf_params.view_scale_factor=slider_value;
    });

    // when the user change the status of the intensity plot checkbox
    checkbox_intensity->onChange([this]() {
        if(checkbox_intensity->isChecked()){
            gf_params.show_intensity=true;
        }
        else{
            gf_params.show_intensity=false;
        }
        


    });
    
    // when the user presses on the "Submit" button
    button_submit->onPress([&](){
        std::string userInput;

        if (radioButton_plane->isChecked()){
            gf_params.initial_wave_form =waveForm::plane;
        }
        else if (radioButton_circular->isChecked()){
            gf_params.initial_wave_form =waveForm::circular;
        }
        else if (radioButton_doubleSlit->isChecked()){
            gf_params.initial_wave_form =waveForm::doubleSlit;
        }
        else{
            std::cerr << "Initial wave form not valid.\nThe problem is in the ManageGUI Class." <<std::endl;
        }
        
        userInput=editBox_speed->getText().toStdString();
        if(!userInput.empty()) gf_params.speed = std::stod(userInput);
        
        userInput=editBox_WL->getText().toStdString();
        if(!userInput.empty()) gf_params.wavelength = std::stod(userInput);

        
        if(gf_params.initial_wave_form==waveForm::doubleSlit || gf_params.initial_wave_form==waveForm::circular)
        {
            userInput=editBox_wave_width->getText().toStdString();
            if(!userInput.empty()) gf_params.slit_width = std::stod(userInput);
            
            userInput=editBox_nsources->getText().toStdString();
            if(!userInput.empty()) gf_params.n_sources = std::stoi(userInput);
        }
        
        if(gf_params.initial_wave_form==waveForm::doubleSlit){
            userInput=editBox_slits_distance->getText().toStdString();
            if(!userInput.empty()) gf_params.slits_distance = std::stod(userInput);
            userInput=editBox_number_of_slits->getText().toStdString();
            if(!userInput.empty()) gf_params.n_slits = std::stoi(userInput);
        }
            
        if(gf_params.initial_wave_form==waveForm::plane){
            userInput=editBox_inf_sources_distance->getText().toStdString();
            if(!userInput.empty()) gf_params.infplane_distance_sources = std::stod(userInput);
        }
        
        if (checkbox_amplitude->isChecked())
            gf_params.amplitude_func =amplitudeForm::costheta;
        else
            gf_params.amplitude_func =amplitudeForm::one;
        
        if (checkbox_intensity->isChecked())
            gf_params.show_intensity =true;
        else
            gf_params.show_intensity =false;
        
        

        was_button_pressed=true;
        
        button_submit->setText(textOutput::getText("submit_button_calc"));
        
        gf_params.view_scale_factor=1;
        slider_scale->setValue(1);
        

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


/*
 * Sets up the the widgets configuration and text for the "Plan wave"option.
 */
void ManageGUI::setPlaneWidgets(){
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);
    gui_to_manage->remove(editBox_nsources);
    gui_to_manage->remove(label_nsources);
    gui_to_manage->remove(editBox_wave_width);
    gui_to_manage->remove(label_wave_width);
    
    gui_to_manage->remove(editBox_number_of_slits);
    gui_to_manage->remove(label_number_of_slits);

    gui_to_manage->add(editBox_inf_sources_distance,"EditBoxInfDistanceSources");
    gui_to_manage->add(label_inf_sources_distance,"LabelInfDistanceSources");

    //label_wave_width->setText(textOutput::getText("label_wave_width_plane"));
    //label_nsources->setText(textOutput::getText("label_nsources"));

    return;
}


/*
 * Sets up the the widgets configuration and text for the "Circular wave" option.
 */
void ManageGUI::setCircularWidgets(){
    gui_to_manage->remove(editBox_inf_sources_distance);
    gui_to_manage->remove(label_inf_sources_distance);
    gui_to_manage->remove(editBox_slits_distance);
    gui_to_manage->remove(label_slits_distance);

    gui_to_manage->remove(editBox_number_of_slits);
    gui_to_manage->remove(label_number_of_slits);
    
    gui_to_manage->add(editBox_nsources,"EditBoxNumberSources");
    gui_to_manage->add(label_nsources,"LabelNumberSources");
    gui_to_manage->add(editBox_wave_width,"EditBoxWaveWidth");
    gui_to_manage->add(label_wave_width,"LabelWaveWidth");
    
    label_wave_width->setText(textOutput::getText("label_wave_width_circular"));
    label_nsources->setText(textOutput::getText("label_nsources"));
    return;
}


/*
 * Sets up the the widgets configuration and text for the "Double slit" option.
 */
void ManageGUI::setDoubleSlitWidgets(){
    gui_to_manage->remove(editBox_inf_sources_distance);
    gui_to_manage->remove(label_inf_sources_distance);

    gui_to_manage->add(editBox_nsources,"EditBoxNumberSources");
    gui_to_manage->add(label_nsources,"LabelNumberSources");
    gui_to_manage->add(editBox_wave_width,"EditBoxWaveWidth");
    gui_to_manage->add(label_wave_width,"LabelWaveWidth");
    
    gui_to_manage->add(editBox_slits_distance,  "EditBoxSlitsDistance");
    gui_to_manage->add(label_slits_distance, "LabelSlitsDistance");

    gui_to_manage->add(editBox_number_of_slits);
    gui_to_manage->add(label_number_of_slits);
    
    label_wave_width->setText(textOutput::getText("label_wave_width_doubleSlit"));
    label_nsources->setText(textOutput::getText("label_nsources_doubleSlit"));
}

/*
 * Is called to update the contextual widgets if necessary, after the user selected
 * a new wave form. It has to be called in main() after the calculations are performed to update
 * the submit button text.
 */
void ManageGUI::updateContextualWidgets(){
    unsigned int current_waveForm_option=9999;
    // Checks which option was selected
    if (radioButton_plane->isChecked()){
        current_waveForm_option=waveForm::plane;
    }
    else if (radioButton_circular->isChecked()){
        current_waveForm_option=waveForm::circular;
    }
    else if (radioButton_doubleSlit->isChecked()){
        current_waveForm_option=waveForm::doubleSlit;
    }
    else
    {
        std::cerr << "Wave type selected is not defined.\nCheck ManageGUI::setContextualWidgets() for the error." << std::endl;
        exit(0);
    }
    
    if(was_button_pressed){
        button_submit->setText(textOutput::getText("submit_button"));
        was_button_pressed=false;
    }
    
    // Exit this method if the user has not made any change.
    if(current_waveForm_option==last_waveForm_option)
        return;
    
    // Change the widgets if the user selected a new form
    if(current_waveForm_option==waveForm::plane){
        setPlaneWidgets();
        //button_submit->setPosition(100, initial_submit_yposition-40);
    }
    else if(current_waveForm_option==waveForm::circular){
        setCircularWidgets();
        //button_submit->setPosition(100, initial_submit_yposition);
    }
    else if(current_waveForm_option==waveForm::doubleSlit){
        setDoubleSlitWidgets();
        //button_submit->setPosition(100, initial_submit_yposition+35);
    }
    
    last_waveForm_option=current_waveForm_option;
    

        
    return;
}


/*
 * Creates all the widgets, sets them up (position, size, etc) and adds them to the GUI.
 * At the end, setTextInGUI is called to set the text in the widget.
 */

void ManageGUI::setGeneralWidgets(){
    unsigned int widget_y_position=0;

    widget_y_position+=10;
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
    radioButton_circular=tgui::RadioButton::create();
    radioButton_circular->setPosition(20,widget_y_position);
    radioButton_circular->setTextClickable(true);
    gui_to_manage->add(radioButton_circular, "RadioButtonSphere");

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
    editBox_WL->setText(keepNDecimals(gf_params.wavelength,1));
    editBox_WL->setInputValidator(tgui::EditBox::Validator::Float);
    gui_to_manage->add(editBox_WL, "EditBoxWavelength");
    
    label_WL = tgui::Label::create();
    label_WL->setPosition(120, widget_y_position+7);
    label_WL->setTextSize(FONT_SIZE);
    gui_to_manage->add(label_WL, "LabelWavelenght");

    
    widget_y_position+=35;
    editBox_speed = tgui::EditBox::create();
    editBox_speed->setSize(100, 30);
    editBox_speed->setPosition(10,widget_y_position);
    editBox_speed->setText(keepNDecimals(gf_params.speed,1));
    editBox_speed->setInputValidator(tgui::EditBox::Validator::Float);
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
    if(gf_params.initial_wave_form==waveForm::doubleSlit || gf_params.initial_wave_form==waveForm::circular)
        gui_to_manage->add(editBox_nsources, "EditBoxNumberSources");

    label_nsources = tgui::Label::create();
    label_nsources->setPosition(120, widget_y_position+6);
    label_nsources->setTextSize(FONT_SIZE);
    if(gf_params.initial_wave_form==waveForm::doubleSlit || gf_params.initial_wave_form==waveForm::circular)
        gui_to_manage->add(label_nsources, "LabelNumberSources");
    
    editBox_inf_sources_distance = tgui::EditBox::create();
    editBox_inf_sources_distance->setSize(100, 30);
    editBox_inf_sources_distance->setPosition(10, widget_y_position);
    editBox_inf_sources_distance->setText(std::to_string(gf_params.infplane_distance_sources));
    editBox_inf_sources_distance->setInputValidator(tgui::EditBox::Validator::UInt);
    if(gf_params.initial_wave_form==waveForm::plane)
        gui_to_manage->add(editBox_inf_sources_distance, "EditBoxInfDistanceSources");

    label_inf_sources_distance = tgui::Label::create();
    label_inf_sources_distance->setPosition(120, widget_y_position+6);
    label_inf_sources_distance->setTextSize(FONT_SIZE);
    if(gf_params.initial_wave_form==waveForm::plane)
        gui_to_manage->add(label_inf_sources_distance, "LabelInfDistanceSources");
    
    widget_y_position+=35;
    editBox_wave_width = tgui::EditBox::create();
    editBox_wave_width->setSize(100, 30);
    editBox_wave_width->setPosition(10, widget_y_position);
    editBox_wave_width->setText(keepNDecimals(gf_params.slit_width,1));
    editBox_wave_width->setInputValidator(tgui::EditBox::Validator::Float);
    if(gf_params.initial_wave_form==waveForm::doubleSlit || gf_params.initial_wave_form==waveForm::circular)
        gui_to_manage->add(editBox_wave_width, "EditBoxWaveWidth");

    label_wave_width= tgui::Label::create();
    label_wave_width->setPosition(120, widget_y_position);
    label_wave_width->setTextSize(FONT_SIZE);
    if(gf_params.initial_wave_form==waveForm::doubleSlit || gf_params.initial_wave_form==waveForm::circular)
        gui_to_manage->add(label_wave_width, "LabelWaveWidth");
    
    //initial_submit_yposition=widget_y_position+40;

    widget_y_position+=35;
    editBox_slits_distance=tgui::EditBox::create();
    editBox_slits_distance->setSize(100, 30);
    editBox_slits_distance->setPosition(10, widget_y_position);
    editBox_slits_distance->setText(keepNDecimals(gf_params.slits_distance,1));
    editBox_slits_distance->setInputValidator(tgui::EditBox::Validator::Float);
    if(gf_params.initial_wave_form==waveForm::doubleSlit)
        gui_to_manage->add(editBox_slits_distance, "EditBoxSlitsDistance");

    
    label_slits_distance=tgui::Label::create();
    label_slits_distance->setPosition(120,widget_y_position+6);
    label_slits_distance->setTextSize(FONT_SIZE);
    if(gf_params.initial_wave_form==waveForm::doubleSlit)
        gui_to_manage->add(label_slits_distance, "LabelSlitsDistance");

    widget_y_position+=35;
    editBox_number_of_slits=tgui::EditBox::create();
    editBox_number_of_slits->setSize(100, 30);
    editBox_number_of_slits->setPosition(10, widget_y_position);
    editBox_number_of_slits->setText(std::to_string(gf_params.n_slits));
    editBox_number_of_slits->setInputValidator(tgui::EditBox::Validator::UInt);
    if(gf_params.initial_wave_form==waveForm::doubleSlit)
        gui_to_manage->add(editBox_number_of_slits, "EditBoxNumberOfSlits");

    
    label_number_of_slits=tgui::Label::create();
    label_number_of_slits->setPosition(120,widget_y_position+6);
    label_number_of_slits->setTextSize(FONT_SIZE);
    if(gf_params.initial_wave_form==waveForm::doubleSlit)
        gui_to_manage->add(label_number_of_slits, "LabelNumberOfSlits");
    
    widget_y_position+=40;
    button_submit = tgui::Button::create();
    button_submit->setSize(120, 40);
    button_submit->setPosition(100, widget_y_position);
    gui_to_manage->add(button_submit, "SubmitButton");
    
    widget_y_position+=50;
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
    
    
    widget_y_position+=35;
    label_copyright = tgui::Label::create();
    label_copyright->setPosition(120, widget_y_position);
    label_copyright->setTextSize(FONT_SIZE-2);
    gui_to_manage->add(label_copyright, "LabelCopyright");
    
    setTextInGUI();

}

/*
 * Sets the texting the GUI. It is called at the beginining and when the user changes
 * the prefered language.
 */
void ManageGUI::setTextInGUI(){
    label_option->setText(textOutput::getText("label_option"));
    radioButton_plane->setText(textOutput::getText("radioButton_plane"));
    radioButton_circular->setText(textOutput::getText("radioButton_circular"));
    radioButton_doubleSlit->setText(textOutput::getText("radioButton_doubleSlit"));
    label_parameters->setText(textOutput::getText("label_parameters"));
    checkbox_amplitude->setText(textOutput::getText("checkbox_amplitude"));
    label_WL->setText(textOutput::getText("label_WL"));
    label_speed->setText(textOutput::getText("label_speed"));
    label_nsources->setText(textOutput::getText("label_nsources"));
    label_inf_sources_distance->setText(textOutput::getText("label_inf_sources_distance"));
    label_number_of_slits->setText(textOutput::getText("label_number_of_slits"));
    label_wave_width->setText(textOutput::getText("label_wave_width_plane"));
    label_slits_distance->setText(textOutput::getText("label_slits_distance"));
    button_submit->setText(textOutput::getText("submit_button"));
    label_display->setText(textOutput::getText("label_display"));
    checkbox_intensity->setText(textOutput::getText("checkbox_intensity"));
    label_slider_scale->setText(textOutput::getText("label_slider_scale"));
    label_language->setText(textOutput::getText("label_language"));

    comboBox_language->changeItemById("en",textOutput::getText("comboBox_english"));
    comboBox_language->changeItemById("fr", textOutput::getText("comboBox_french"));
    
    label_copyright->setText(textOutput::getText("label_copyright"));

   
}

#define MAX_SUM_PERIODICITY 100

/*
 * Returns the parameters that the user have entered. It also checks if the values make sense
 * (they should be greater than 0). Also, if the submit button was pressed, it changes the text
 * of it.
 *
 * input_window : The input window that is used to create the widgets.
 *
 * return : The parameters that were set by the user.
 */
GFParameters ManageGUI::getNewParameters()
{
    if(gf_params.initial_wave_form==waveForm::doubleSlit && gf_params.n_slits>1){
        if(gf_params.slit_width>=gf_params.slits_distance){
            showErrorWindow(textOutput::getText("error_doubleSlit"));
            
            gf_params.slits_distance=2*gf_params.slit_width;
            editBox_wave_width->setText(keepNDecimals( gf_params.slit_width,1));
            editBox_slits_distance->setText(keepNDecimals( gf_params.slits_distance,1));
        }
    }
    
    if(gf_params.initial_wave_form==waveForm::plane){
        FLOAT periodicity=getSumPeriodicity(gf_params.infplane_distance_sources, gf_params.wavelength);
        if( periodicity > MAX_SUM_PERIODICITY+0.1){
            gf_params.wavelength= changeLambdaForPeriodicity(gf_params.infplane_distance_sources, gf_params.wavelength, MAX_SUM_PERIODICITY);
            editBox_WL->setText(keepNDecimals(gf_params.wavelength,2));
            showErrorWindow(textOutput::getText("error_wl_change"));

        }
            
    }

    if(gf_params.n_sources<1){
        gf_params.n_sources=1;
        editBox_nsources->setText(std::to_string( gf_params.n_sources));
    }
    
    if(gf_params.wavelength<=0){
        gf_params.wavelength=10;
        editBox_WL->setText(keepNDecimals( gf_params.wavelength,1));
    }

    if(gf_params.speed <=0){
        gf_params.speed=10;
        editBox_speed->setText(keepNDecimals( gf_params.speed,1));
    }
    
    if(gf_params.slit_width<=0){
        gf_params.slit_width=10;
        editBox_wave_width->setText(keepNDecimals( gf_params.slit_width,1));
    }
    
    if(gf_params.slits_distance<=0){
        gf_params.slits_distance=10;
        editBox_slits_distance->setText(keepNDecimals( gf_params.slits_distance,1));
    }
    
    return gf_params;
}


/*
 * Creates a window with a error message and a "Ok" button.
 *
 * errorMessage : A string containing the error message.
 */

void ManageGUI::showErrorWindow(std::string errorMessage) {
    sf::RenderWindow errorWindow(sf::VideoMode({400, 250}), "Erreur", sf::Style::Titlebar | sf::Style::Close);
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
