#include <iostream>

#include "textOutput.h"



namespace textOutput {
    std::string current_language = "fr"; // Default language
    std::map<std::string,std::map<std::string,std::string>> text_languages;

    // Language-specific texts



    
    void initializeText(){
        text_languages["fr"] = {
            {"checkbox_amplitude", "Dépendance en θ de l'amplitude"},
            {"checkbox_power", "Afficher l'intensité seulement"},
            
            {"label_WL", "Longueur d'onde (en pixels)"},
            {"label_speed","Vitesse de l'onde\n(pixels/seconde)"},
            {"label_nsources","Nombre de sources"},
            {"label_wave_width_plane","Largeur de l'onde plane plane\ninitiale (en pixels)"},
            {"label_wave_width_spherical","Circonférence de l'onde\nsphérique initiale (en pixels)"},
            {"label_wave_width_doubleSlit","Largeur de chaque fente\n(en pixels)"},
            {"label_nsources","Nombre de sources"},
            {"label_nsources_doubleSlit","Nombre de sources par fente"},
            
            {"submit_button","Soumettre"},
            {"submit_button_calc","Calcul en cours..."},

            
            {"label_slits_distance","Distance entre les fentes\n(centre à centre)"},
            {"label_option","Choisir la forme de l'onde initiale:"},
            {"label_parameters","Définir les paramètres:"}, 
            
            {"radioButton_plane","Onde plane (1 fente)"},
            {"radioButton_spherical","Onde sphérique"},
            {"radioButton_doubleSlit","Deux fentes"},

            {"error_doubleSlit","La distance entre les fentes (de\ncentre à centre) devrait être plus\ngrande que la largeur des fentes.\n Le simulateur va générer une seule onde\n plane (1 fente)."}
        };

        text_languages["en"]  = {
            {"checkbox_amplitude", " θ dependency of the sources amplitude"},
            {"checkbox_power", "Show the intensity only"},

            
            {"label_WL", "Wavelenght (in pixels)"},
            {"label_speed","Wave speed\n(pixels/second)"},
            {"label_nsources","Number of sources"},
            {"label_wave_width_plane","Width of the initial\nplane wave (in pixels)"},
            {"label_wave_width_spherical","Initial wave circonference\n(in pixels)"},
            {"label_wave_width_doubleSlit","Slit width (in pixel)"},
            {"label_nsources","Number of sources"},
            {"label_nsources_doubleSlit","Number of sources per slit"},
            
            {"submit_button","Submit"},
            {"submit_button_calc","Calculating..."},

            {"label_slits_distance","Distance between the slits\n(center to center)"},
            {"label_option","Select the form of the initial wave:"},
            {"label_parameters","Set the parameters"}, 

            {"radioButton_plane","Plane wave (1 slit)"},
            {"radioButton_spherical","Spherical wave"},
            {"radioButton_doubleSlit","Double slit"},        

            {"error_doubleSlit","The distance between the slits\n(from center to center) should be\ngreater than the slits width. This\nprogram will consider just 1 slit\nwith double width."}
        };
    }

    // Function to set language
    void setLanguage(const std::string& lang) {
        if(text_languages.empty()) initializeText();
    
        if (text_languages.find(lang) == text_languages.end()) {
            std::cerr<<"Language "<<lang<<" not supported. The language will be set to English."<<std::endl;
            current_language="en";
        }
        else{
            current_language=lang;
        }
    }

    // Function to output text based on current language
    std::string getText(const std::string& key) {
        if(text_languages.empty()) initializeText();
        
        if (text_languages[current_language].find(key) == text_languages[current_language].end()) {
            std::cerr << "The text does not exist for " << key <<" in "<< current_language<< "\nCheck in textOutput.cpp" <<std::endl;
            return std::string("Text not defined.") +key;
        }
        return text_languages[current_language][key];
    }
}
