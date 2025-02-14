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



int main()
{
    clock_t start_time = clock();
    
    unsigned int main_win_width, main_win_height, input_win_width,input_win_height;
    
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    
    input_win_width=350;
    input_win_height=400;
    main_win_height=(unsigned int)(0.85*float(desktop.size.y));
    main_win_width=desktop.size.x -input_win_width;
    
    
    sf::RenderWindow mainWindow(sf::VideoMode({main_win_width, main_win_height}), "Principe de Huygens");

    mainWindow.setPosition(sf::Vector2i(0, 0));

    sf::RenderWindow inputWindow(sf::VideoMode({input_win_width,input_win_height}), "Parametres");

    inputWindow.setPosition(sf::Vector2i(main_win_width, 0));

    
    sf::Texture texture(sf::Vector2u(main_win_width, main_win_height)); // Throws sf::Exception if an error occurs
    
    tgui::Gui gui(inputWindow);
 
        
    auto [width, height] = texture.getSize();

    std::vector<std::uint8_t> pixels(width * height * 4, 255);

    
    GFParameters gf_params;
    gf_params.speed=SPEED;
    gf_params.wavelength=WAVELENGTH;
    gf_params.amplitude_func=amplitudeForm::one;
    gf_params.initial_wave_form=waveForm::plane;
    gf_params.slit_width=800;
    gf_params.slits_distance=0;
    gf_params.n_sources=20;
    
    GridField gf(width, height, gf_params);
    
    ManageGUI manage_GUI(gui,gf.getParameters());
     
     
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
        GFParameters new_parameters = manage_GUI.getNewParameters(inputWindow);
        if(!(new_parameters==gf_params))
        {
            gf.initializeParamAndGrid(new_parameters);
            gf_params=gf.getParameters();
            
        }
        
        manage_GUI.updateContextualWidgets();
        


        FLOAT time_elapsed = FLOAT(clock()-start_time)/FLOAT(CLOCKS_PER_SEC);

        gf.getFieldValues(wave_values_table, time_elapsed);
        
        for(unsigned int i=0; i<wave_values_table.size();i++){
                unsigned char amp = wave_values_table[i];
                pixels[i*4 +0]=amp;
                pixels[i*4 +1]=amp;
                pixels[i*4 +2]=amp;
        }
        drawSourcesScreen(pixels, width,height, gf.ws);
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

