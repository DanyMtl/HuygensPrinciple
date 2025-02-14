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

#define FLOAT double


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

class GFParameters
{
public:
    unsigned int wavelength;
    unsigned int speed;
    unsigned int slits_distance;
    unsigned int amplitude_func;
    unsigned int slit_width;
    unsigned int initial_wave_form;
    unsigned int n_sources;
    
    bool show_power;
    
    GFParameters();
    void resetValues();
    bool checkValuesSetCorrectly();
    bool checkEmpty();
    
    bool operator==(const GFParameters& gfp_compare) const;

    bool checkIfGridChange(const GFParameters& gfp_compare);
    
    void printParameters();

};

//**************************************************
//
// WaveSources class is used to manage the sources that produce the waves.
//
//**************************************************

class WaveSources{
private:
    std::vector<std::pair<FLOAT, FLOAT>> sources_coord;
    std::vector<std::pair<FLOAT, FLOAT>> direction_wave;
    
    unsigned int screen_width;  // the width of the screen where the sources will be created
    unsigned int screen_height; // the height of the screen where the sources will be created
    unsigned int slit_width;
    unsigned int initial_wave_form;
    
    unsigned int slits_distance;
    unsigned int amplitude_func;
    
public:
    WaveSources();
    
    void setAmplitudeForm(unsigned int input_amplitude_func);
    unsigned int getAmplitudeForm();
    
    void setScreenSize(unsigned int input_width, unsigned int input_height );
    
    void reinitializePlane( unsigned int n_sources,unsigned int input_slit_width);
    
    void reinitializeCircular( unsigned int n_sources,unsigned int input_slit_width);
    
    void reinitializeDoubleSlit( unsigned int n_sources,unsigned int input_slit_width, unsigned int input_slits_distance);
    
    FLOAT x(int n);
    
    FLOAT y(int n);
    
    // Prints the sources coordinates  on the screen
    void print();
    
    unsigned int getNSources();
    
    bool isRightDirection(int x, int y, unsigned int n);
    
    FLOAT cosAngleKR(FLOAT x, FLOAT y, int n);
    
    unsigned int getSourcesWidth();
    
    unsigned int getSlitsDistance();
    
    unsigned int getInitialWaveForm();
    
    
    
};

//**************************************************
//
// The GridField class is used to manage the calculation an array
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
     
class GridField{
private:
    
    unsigned int screen_width;
    unsigned int screen_height;
    
    std::vector<FLOAT> grid_phase;
    std::vector<FLOAT> grid_amplitude;
    std::vector<FLOAT> grid_power;
    
    FLOAT k;
    FLOAT w;
    FLOAT period;
    
    GFParameters parameters;
    
public:
    WaveSources ws;
    
    void setAmplitudeForm(unsigned int input_amplitude_func);
    unsigned int getAmplitudeForm();

    GridField(int input_screen_width, int input_screen_height,GFParameters params);
    void initializeParamAndGrid(GFParameters new_parameters);
    
    void initialize_grid();
    
    void getFieldValues(std::vector<unsigned char> & y_values,FLOAT time);
    
    GFParameters getParameters();
    
    unsigned int getWaveLength();
      
    FLOAT getWavePeriod();
    
    unsigned int getNSources();
    
    unsigned int getDistanceSources();
    
    unsigned int getSourcesWidth();
    
    unsigned int getSlitsDistance();
    
    
};




#endif // WAVECLASSES_H
