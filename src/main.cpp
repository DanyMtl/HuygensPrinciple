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
#include "WaveClasses.h"
#include "GUIClass.h"


#define WIDTH 1000
#define HEIGHT 700

#define WAVELENGTH 50
#define SPEED 30






//**************************************************
//
// The drawSourcesScreen function takes a table of pixels to draw
// on the screen and adds the position of the sources in red.
//
//**************************************************

void drawSourcesScreen(std::vector<std::uint8_t> &pix_table, unsigned int width, unsigned int height, WaveSources ws){
    if(ws.getNSources()<=0) {std::cerr << "No sources in WaveSources" << std::endl; exit(0);}
    
    //auto [width, height] = pix_table.getSize();
    for(unsigned int n=0;n<ws.getNSources();n++){
        int x=std::round(ws.x(n));
        int y=std::round(ws.y(n));
        for(int j=y-2; j<=y+2;j++ ){
            if(j<0 || j>= (int)height) continue;
            for(int i=x-2; i<=x+2;i++ ){
                if(i<0 || i>= (int)width) continue;

                unsigned int pointer_pos=(j*width+i)*4;
                pix_table[pointer_pos+0]= 255;
                pix_table[pointer_pos+1]= 0;
                pix_table[pointer_pos+2]= 0;
                pix_table[pointer_pos+3]= 255;
            }
        }
    }
}

int close_error_window;
void showErrorWindow(std::string errorMessage) {
    sf::RenderWindow errorWindow(sf::VideoMode({300, 200}), "Erreur", sf::Style::Titlebar | sf::Style::Close);
    tgui::Gui gui(errorWindow);
    
    auto label_err_msg=tgui::Label::create(errorMessage);
    
    label_err_msg->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
    label_err_msg->setVerticalAlignment(tgui::VerticalAlignment::Center);
    label_err_msg->setTextSize(15);
    
    gui.add(label_err_msg);
    
    tgui::Layout2d windowSize = gui.getView().getSize();
    float label_x_pos=(gui.getView().getSize().x - label_err_msg->getSize().x)/2;
    float label_y_pos=(gui.getView().getSize().y - label_err_msg->getSize().y)/2-30;
   
    label_err_msg->setPosition(label_x_pos,label_y_pos);
    
    
    auto button = tgui::Button::create("Ok");
    button->setSize(60, 40);

    float button_x_pos=(gui.getView().getSize().x - button->getSize().x)/2;
    float button_y_pos=label_y_pos+label_err_msg->getSize().y+30;


    button->setPosition(button_x_pos, button_y_pos);
    gui.add(button);

    button->onPress([&](){
        errorWindow.close();
    });
    
    while (errorWindow.isOpen()) {
        while (const std::optional event = errorWindow.pollEvent()){
            // "close requested" event: we close the mainWindow
            if (event->is<sf::Event::Closed>())//|| close_error_window==1)
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


int main()
{
    clock_t start_time = clock();
    
    
    sf::RenderWindow mainWindow(sf::VideoMode({WIDTH, HEIGHT}), "Principe de Huygens");

    mainWindow.setPosition(sf::Vector2i(0, 0));

    sf::RenderWindow inputWindow(sf::VideoMode({350,400}), "Parametres");

    inputWindow.setPosition(sf::Vector2i(WIDTH+10, 20));

    sf::Texture texture(sf::Vector2u(WIDTH, HEIGHT)); // Throws sf::Exception if an error occurs
    
    tgui::Gui gui(inputWindow);
 
        
    auto [width, height] = texture.getSize();
    
    std::vector<std::uint8_t> pixels(width * height * 4, 255);


    GridWave gw(width, height, WAVELENGTH,SPEED,20,1000 ,waveForm::plane,200);
    
    ManageGUI manage_GUI(gui,gw);
     
     
    std::vector<unsigned char> wave_values_table(width * height, 0);
    
  
    
    // run the program as long as the mainWindow is open
    while (mainWindow.isOpen() && inputWindow.isOpen())
    {
        // check all the inputWindow's events that were triggered since the last iteration of the loop
        while (const std::optional event = inputWindow.pollEvent()) {
            if (event->is<sf::Event::Closed>()){
            
                inputWindow.close();
            }
            
            gui.handleEvent(*event);

        }
        // check all the mainWindow's events that were triggered since the last iteration of the loop

        while (const std::optional event = mainWindow.pollEvent())
        {
            
            // "close requested" event: we close the mainWindow
            if (event->is<sf::Event::Closed>())
            {
                mainWindow.close();
                inputWindow.close();
            }
        }
        
        // Checks if new parameters when set by the user and returns it.
        std::vector<int> newparam = manage_GUI.getNewParameters(inputWindow);
        if(!newparam.empty())
        {
            gw.setAmplitudeForm(newparam[6]);
            gw.initializeParam(newparam[0], newparam[3], newparam[1], newparam[2], newparam[4], newparam[5]);
            if(newparam[4]==waveForm::doubleSlit){
                if(newparam[2]>=newparam[5]){
                    showErrorWindow("La distance entre les fentes (de\ncentre à centre) devrait être plus\ngrande que la largeur des fentes.\n Le simulateur va générer une onde\n plane (1 fente).");
                    
                }
            }
            
        }
        
        manage_GUI.updateContextualWidgets();
        


        float time_elapsed = float(clock()-start_time)/float(CLOCKS_PER_SEC);

        gw.getWaveValues(wave_values_table, time_elapsed);
        
        for(unsigned int i=0; i<wave_values_table.size();i++){
                unsigned char amp = wave_values_table[i];
                pixels[i*4 +0]=amp;
                pixels[i*4 +1]=amp;
                pixels[i*4 +2]=amp;
        }
        drawSourcesScreen(pixels, width,height, gw.ws);
        manage_GUI.buttonTextBack();

        mainWindow.clear(sf::Color::Black);

        inputWindow.clear(sf::Color(255,255,255));
        
        texture.update(pixels.data());

        sf::Sprite sprite(texture);
        
        gui.draw();
        mainWindow.draw(sprite);
        
        inputWindow.display();
        mainWindow.display();
    }
    return 0;
}

