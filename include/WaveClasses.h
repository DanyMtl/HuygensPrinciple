#ifndef WAVECLASSES_H // Include guard to prevent multiple inclusions
#define WAVECLASSES_H

#include <random>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <string>
#include <complex>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>

namespace waveForm
{
    const unsigned int plane=0;
    const unsigned int spherical=1;
    const unsigned int doubleSlit=2;
}

namespace amplitudeForm
{
    const unsigned int one=0;
    const unsigned int costheta=1;
}

//**************************************************
//
// WaveSources class is used to manage the sources that produce the waves.
//
//**************************************************

class WaveSources{
private:
    std::vector<std::pair<float, float>> sources_coord;
    std::vector<std::pair<float, float>> direction_wave;
    
    unsigned int screen_width;  // the width of the screen where the sources will be created
    unsigned int screen_height; // the height of the screen where the sources will be created
    unsigned int wave_width;
    unsigned int initial_wave_form;
    
    unsigned int slits_distance;
    unsigned int amplitude_func;
    
public:
    WaveSources();
    
    void setAmplitudeForm(unsigned int input_amplitude_func);
    unsigned int getAmplitudeForm();
    
    void setScreenSize(unsigned int input_width, unsigned int input_height );
    
    void reinitializePlane( unsigned int n_sources,unsigned int input_wave_width);
    
    void reinitializeCircular( unsigned int n_sources,unsigned int input_wave_width);
    
    void reinitializeDoubleSlit( unsigned int n_sources,unsigned int input_wave_width, unsigned int input_slits_distance);
    
    float x(int n);
    
    float y(int n);
    
    // Prints the sources coordinates  on the screen
    void print();
    
    unsigned int getNSources();
    
    bool isRightDirection(int x, int y, unsigned int n);
    
    float cosAngleKR(float x, float y, int n);
    
    unsigned int getSourcesWidth();
    
    unsigned int getSlitsDistance();
    
    
    
};

//**************************************************
//
// The GridWave class is used to manage the calculation an array
// of values of the field generated on a screen by all the wave sources.
//
// The object has to be initialize when created.
//
// It precalculates a grid with the amplitude and the phase of the
// spatial part of the total wave:  Σ e^(ikx)/r
// For each timestep, it just needs to add the part of the phase
// associated to time (ωt).
//
//**************************************************
     
class GridWave{
private:
    
    unsigned int screen_width;
    unsigned int screen_height;
    
    std::vector<float> grid_phase;
    std::vector<float> grid_amplitude;
    
    float k;
    float w;
    unsigned int wavelength;
    float period;
    unsigned int speed;
    
public:
    WaveSources ws;
    
    void setAmplitudeForm(unsigned int input_amplitude_func);
    unsigned int getAmplitudeForm();

    GridWave(int input_screen_width, int input_screen_height, int input_wavelength,int input_speed, int n_sources, int width_sources, unsigned int wave_form, unsigned int input_distance_slits);
    // This method can be used to reinitialize the parameters when they are changed.
    void initializeParam(int input_wavelength, int input_speed, int input_n_sources, int width_wave, unsigned int wave_form,unsigned int input_distance_slits);
    
    
    void initialize_grid();
    
    void getWaveValues(std::vector<unsigned char> & y_values,float time);
    
    
    unsigned int getWaveLength();
      
    float getWavePeriod();
    
    unsigned int getNSources();
    
    unsigned int getDistanceSources();
    
    unsigned int getSourcesWidth();
    
    unsigned int getSlitsDistance();
};




#endif // WAVECLASSES_H
