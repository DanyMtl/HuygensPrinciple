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


int main()
{
    clock_t start_time = clock();
    
    unsigned int main_win_width, main_win_height, input_win_width,input_win_height;
    
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    
    input_win_width=350;
    input_win_height=690;
    main_win_height=(unsigned int)(0.85*float(desktop.size.y));
    main_win_width=desktop.size.x -input_win_width;
    
    
    sf::RenderWindow mainWindow(sf::VideoMode({main_win_width, main_win_height}), "Principe de Huygens",sf::Style::Default);
    sf::RenderWindow inputWindow(sf::VideoMode({input_win_width,input_win_height}), "Parametres",sf::Style::Default);
    mainWindow.setPosition(sf::Vector2i(0, 0));
    inputWindow.setPosition(sf::Vector2i(main_win_width, 0));


    /* This part of the code is necessary to fix a problem on Mac. When tested on a
     * MacBook Air M2, the classes that manage the input from users (IMKClient
     * and IMKInputSession) do not usually start unless we focus on another
     * window and come back to the program. By calling the pollEvent() early, it seems
     * to fix the problem and it does not require much ressources.
     */
    for(unsigned int i=0;i<10; i++)
    {
        (void) inputWindow.pollEvent();
        (void) mainWindow.pollEvent();
        sf::sleep(sf::seconds(0.05)); 
    }
    
    
    sf::Texture texture(sf::Vector2u(main_win_width, main_win_height)); // Throws sf::Exception if an error occurs
    auto [width, height] = texture.getSize();

    
    std::vector<std::uint8_t> pixels(width * height * 4, 255);
    
    
    GFParameters gf_params;
    gf_params.setDefaultParameters();
    GridField gf(width, height, gf_params);
    
    tgui::Gui gui(inputWindow);
    
    ManageGUI manage_GUI(gui,gf.getParameters());


    // run the program as long as the mainWindow is open
    while (mainWindow.isOpen() && inputWindow.isOpen())
    {
        // check all the inputWindow's events that were triggered since the last iteration of the loop
        while (const std::optional event = inputWindow.pollEvent()) {
            gui.handleEvent(*event);
            
            if (event->is<sf::Event::Closed>()){
                inputWindow.close();
            }

        }
        // check all the mainWindow's events that were triggered since the last iteration of the loop

        while (const std::optional event = mainWindow.pollEvent())
        {
            // "close requested" event: we close the mainWindow
            if (event->is<sf::Event::Closed>()){
                mainWindow.close();
                inputWindow.close();
            }
        }
        
        // Checks if new parameters when set by the user and returns it.
     
        GFParameters new_parameters = manage_GUI.getNewParameters();
        
        // The GUI window needs to be updated here to make sure to change
        // the submit button text if calculations were launched.
        inputWindow.clear(sf::Color(255,255,255));
        gui.draw();
        inputWindow.display();
       
        if(!(new_parameters==gf_params))
        {
            gf.initializeParamAndGrid(new_parameters);
            gf_params=gf.getParameters();
        }
        
        FLOAT time_elapsed = FLOAT(clock()-start_time)/FLOAT(CLOCKS_PER_SEC);

        gf.getPixelsValues(pixels, time_elapsed);

        gf.drawSourcesScreen(pixels);
        gf.drawScaleScreen(pixels);
        manage_GUI.updateContextualWidgets();

        texture.update(pixels.data());
        sf::Sprite sprite(texture);
        
        
        mainWindow.clear(sf::Color::White);
        mainWindow.draw(sprite);
        mainWindow.display();
    }
    return 0;
}

