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



ManageGUI::ManageGUI(tgui::Gui &input_gui, GridWave &input_wg){
        
    gui_to_manage=&input_gui;
    wl=input_wg.getWaveLength();
    sources_width=input_wg.getSourcesWidth();
    n_sources=input_wg.getNSources();
    wave_speed= (float)wl/input_wg.getWavePeriod();
    
    editBox1 = tgui::EditBox::create();
    //editBox1->copy(editBox);
    editBox1->setSize(100, 30);
    editBox1->setPosition(10, 10);
    editBox1->setDefaultText(std::to_string(wl));
    editBox1->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox1);
    
    label1 = tgui::Label::create("Longueur d'onde (en pixels)");
    label1->setPosition(120, 15);
    label1->setTextSize(14);
    gui_to_manage->add(label1);

    //editBox2->copy(editBox);
            
    editBox2 = tgui::EditBox::create();
    editBox2->setSize(100, 30);
    editBox2->setPosition(10, 50);
    editBox2->setDefaultText(std::to_string(n_sources));
    editBox2->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox2);


    label2 = tgui::Label::create("Nombre de sources ");
    label2->setPosition(120, 55);
    label2->setTextSize(14);
    gui_to_manage->add(label2);
    
    editBox3 = tgui::EditBox::create();
    editBox3->setSize(100, 30);
    editBox3->setPosition(10, 90);
    editBox3->setDefaultText(std::to_string(sources_width));
    editBox3->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox3);

    label3= tgui::Label::create("Largeur occupée par les \nsources (en pixels)");
    label3->setPosition(120, 90);
    label3->setTextSize(14);
    gui_to_manage->add(label3);            
    
    editBox4 = tgui::EditBox::create();
    editBox4->setSize(100, 30);
    editBox4->setPosition(10,130);
    editBox4->setDefaultText(std::to_string(wave_speed));
    editBox4->setInputValidator(tgui::EditBox::Validator::UInt);
    gui_to_manage->add(editBox4);

    label4= tgui::Label::create("Vitesse de l'onde\n(pixels/seconde)");
    label4->setPosition(120, 130);
    label4->setTextSize(14);
    gui_to_manage->add(label4);
    
    checkbox = tgui::CheckBox::create("Ondes sphériques");
    checkbox->setPosition(20, 180);
    gui_to_manage->add(checkbox);
     
    button = tgui::Button::create("Soumettre");
    button->setSize(120, 40);
    button->setPosition(100, 210);
    gui_to_manage->add(button);
    

    
    was_button_pressed=false;
        
    button->onPress([&](){
        //int userValue;
        std::string userInput;

        userInput=editBox1->getText().toStdString();
        if(!userInput.empty()) wl = std::stoi(userInput);
        userInput=editBox2->getText().toStdString();
        if(!userInput.empty()) n_sources = std::stoi(userInput);
        userInput=editBox3->getText().toStdString();
        if(!userInput.empty()) sources_width = std::stoi(userInput);
        userInput=editBox4->getText().toStdString();
        if(!userInput.empty()) wave_speed = std::stoi(userInput);
        if (checkbox->isChecked()){
            wave_form=waveForm::spherical;
        }
        else{
            wave_form=waveForm::plane;
        }
        was_button_pressed=true;
    });
        
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
        return {wl,n_sources,sources_width,wave_speed,wave_form};
    }
    else
        return {};
    
}
void ManageGUI::buttonTextBack(sf::RenderWindow &input_window)
{
    button->setText("Soumettre");
    gui_to_manage->draw();


}


