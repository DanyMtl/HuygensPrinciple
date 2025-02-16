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
    const unsigned int circular=1;
    const unsigned int doubleSlit=2;
}

namespace amplitudeForm
{
    const unsigned int one=0;
    const unsigned int costheta=1;
}


//**************************************************
//
// This GFParameters class to store the parameters needed for GridWave
//
//**************************************************

class GFParameters
{
public:
    FLOAT wavelength;
    FLOAT speed;
    FLOAT slits_distance;
    FLOAT slit_width;
    
    unsigned int amplitude_func;
    unsigned int initial_wave_form;
    unsigned int n_sources;
    FLOAT view_scale_factor;
    
    bool show_intensity;
    
    GFParameters();
    void resetValues();
    bool checkValuesSetCorrectly();
    bool checkEmpty();
    
    bool operator==(const GFParameters& gfp_compare) const;

    bool checkIfGridChange(const GFParameters& gfp_compare);
    
    void printParameters();
    
    void setDefaultParameters();
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
    
    unsigned int window_width;  // the width of the screen where the sources will be created
    unsigned int window_height; // the height of the screen where the sources will be created
    FLOAT slit_width;
    FLOAT initial_wave_form;
    
    FLOAT slits_distance;
    unsigned int amplitude_func;
    
public:
    WaveSources();
    
    void setAmplitudeForm(unsigned int input_amplitude_func);
    
    void setScreenSize(unsigned int input_width, unsigned int input_height );
    
    void reinitializePlane( unsigned int n_sources,FLOAT input_slit_width);
    
    void reinitializeCircular( unsigned int n_sources,FLOAT circonference);
    
    void reinitializeDoubleSlit( unsigned int n_sources,FLOAT input_slit_width, FLOAT input_slits_distance);
    
    FLOAT x(int n);
    
    FLOAT y(int n);
    
    // Prints the sources coordinates  on the screen
    void print();
    
    unsigned int getNSources();
    
    bool isRightDirection(int x, int y, unsigned int n);
    
    FLOAT cosAngleKR(FLOAT x, FLOAT y, int n);
        
    
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
    
    unsigned int window_width;
    unsigned int window_height;
    
    std::vector<FLOAT> grid_phase;
    std::vector<FLOAT> grid_amplitude;
    
    FLOAT k;
    FLOAT w;
    FLOAT period;
    
    GFParameters parameters;
    
    WaveSources ws;
    
    const std::vector<unsigned char> RGB_amplitude_max={23,116,219};
    const std::vector<unsigned char> RGB_amplitude_zero={255,255,255};
    const std::vector<unsigned char> RGB_amplitude_min={255,124,20};
    
    const std::vector<unsigned char> RGB_amplitude_text={0,0,0};

    const std::vector<unsigned char> RGB_intensity_max={255,255,255};
    const std::vector<unsigned char> RGB_intensity_zero={0,0,0};
    
    const std::vector<unsigned char> RGB_intensity_text={255,122,0};
    
    const std::vector<unsigned char> RGB_sources={0,190,0};

    

    void addImage(std::vector<std::uint8_t> &pix_table, unsigned int x_start, unsigned int y_start,std::vector<std::vector<unsigned char>>  image_vector, std::vector<unsigned char> rgb_text);
public:

    
    GridField(int input_window_width, int input_window_height,GFParameters params);
    void initializeParamAndGrid(GFParameters new_parameters);
    
    void initialize_grid();
    
    void getPixelsValues(std::vector<std::uint8_t> & pixels_values,FLOAT time);
    
    GFParameters getParameters();
    
    void drawSourcesScreen(std::vector<std::uint8_t> &pix_table);
    void drawScaleScreen(std::vector<std::uint8_t> &pix_table);
    void drawScaleScreenAmplitude(std::vector<std::uint8_t> &pix_table);
    void drawScaleScreenIntensity(std::vector<std::uint8_t> &pix_table);
    unsigned int getNSources();
        
    void testCalculations();

};




#endif // WAVECLASSES_H
