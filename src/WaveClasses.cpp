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

#define PI 3.14159265358979323846

#define COS_DIRECTION true

//**************************************************
//
// WaveSources class is used to manage the sources that produce the waves.
//
//**************************************************



    
WaveSources::WaveSources(){
    screen_width=0;
    screen_height=0;
    sources_coord.push_back({0.0,0.0});
}
    
// Sets the size of the screen
void WaveSources::setScreenSize(unsigned int input_width, unsigned int input_height ){
    screen_width=input_width;
    screen_height=input_height;
}
    
// Initializes the sources by giving the number of sources and the distance between them
void WaveSources::reinitializePlane( unsigned int n_sources,unsigned int input_sources_width){
    initial_wave_form=waveForm::plane;
    if(screen_width==0 || screen_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    sources_width=input_sources_width;
    
    float distance;
    if(n_sources>1)
        distance=float(sources_width)/float(n_sources-1);
    else{
        distance=0;
        sources_width=0;
    }
    sources_coord.clear();
    direction_wave.clear();
    
    float start_x_coord=float(int(screen_width)-int(sources_width))/2.0;
    
    for(unsigned int n=0;n<n_sources;n++){
        float x_coord=start_x_coord+float(n)*distance;
        float y_coord=float(screen_height)-10;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
    }
}

void WaveSources::reinitializeCircular( unsigned int n_sources,unsigned int input_sources_width){
    initial_wave_form=waveForm::plane;
    float radius=float(input_sources_width)/2.0/PI;
    if(screen_width==0 || screen_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    sources_coord.clear();
    direction_wave.clear();
    
    sources_width=input_sources_width;

    
    float start_x_coord=float(screen_width)/2;
    float start_y_coord=float(screen_height)/2;
    
    
    for(unsigned int n=0;n<n_sources;n++){
        float angle=n*2*PI/float(n_sources);
        float rel_x=radius*std::cos(angle);
        float rel_y=radius*std::sin(angle);
        float x_coord=start_x_coord+rel_x;
        float y_coord=start_y_coord+rel_y;
        float norm_rel_r=hypot(rel_x,rel_y);
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({rel_x/norm_rel_r,rel_y/norm_rel_r});
    }
}

// Gives the x coordinate of the source n
float WaveSources::x(int n){
    return sources_coord[n].first;
}

// Gives the y coordinate of the source n
float WaveSources::y(int n){
    return sources_coord[n].second;
}

// Prints the sources coordinates  on the screen
void WaveSources::print(){
    for(unsigned int n=0;n<sources_coord.size();n++){
        std::cout <<"Source #" << n << " :" << x(n) << " , " << y(n) << std::endl;
    }
}

unsigned int WaveSources::getNSources(){return (unsigned int)sources_coord.size(); }

bool WaveSources::isRightDirection(int x, int y, unsigned int n){
   // return true;
    if(x*direction_wave[n].first +y*direction_wave[n].second >0) return true;
    else return false;
}

float WaveSources::cosAngleKR(float x, float y, int n){
    if(COS_DIRECTION==true)
        return (x*direction_wave[n].first +y*direction_wave[n].second)/std::hypot(x,y);
    else
        return 1.0;
}

int WaveSources::getSourcesWidth(){
    return sources_width;
}


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
 



// Initialize the grid for the calculations of the field values for each pixel of the screen
GridWave::GridWave(int input_screen_width, int input_screen_height, int input_wavelength,int input_speed, int n_sources, int width_sources, unsigned int wave_form){

    screen_width=input_screen_width;
    screen_height=input_screen_height;
    ws.setScreenSize(screen_width,screen_height);
    grid_phase.resize(screen_width*screen_height,0);
    grid_amplitude.resize(screen_width*screen_height,0);
    
    
    initializeParam(input_wavelength,input_speed,n_sources, width_sources, wave_form);


    initialize_grid();

}

// This method can be used to reinitialize the parameters when they are changed.
void GridWave::initializeParam(int input_wavelength, int input_speed, int input_n_sources, int width_sources, unsigned int wave_form){
    
    wavelength=input_wavelength;
    speed=input_speed;
    period=float(wavelength)/float(speed);
    k=2*PI/((float)wavelength);
    w=2*PI/period;
    //ws.reinitializePlaneWave(input_n_sources, width_sources);
    if(wave_form==waveForm::plane){
        ws.reinitializePlane(input_n_sources, width_sources);
    }
    else if(wave_form==waveForm::spherical){
        ws.reinitializeCircular(input_n_sources, width_sources);
    }
    else{
        std::cerr << wave_form << " type of wave is not compatible with the GridWave class.\n";
    }
    initialize_grid();
}



// Initialize the arrays for the phases and the amplitudes of the spatial part of the sum of the waves.
void GridWave::initialize_grid(){
    if(grid_phase.size()!=screen_width*screen_height) grid_phase.resize(screen_width*screen_height,0);
    if(grid_amplitude.size()!=screen_width*screen_height) grid_amplitude.resize(screen_width*screen_height,0);
    float abs_average=0;
    for( int j=0;j<(int)screen_height;j++){
        for( int i=0;i<(int)screen_width;i++){

            unsigned int pos_point=(unsigned int)(j*screen_width+i);
            float real_sum=0;
            float imag_sum=0;
            
            for(unsigned int n=0;n<ws.getNSources();n++){
                
                float rel_x=float(i)-ws.x(n);
                float rel_y=float(j)-ws.y(n);
                if(!ws.isRightDirection(rel_x, rel_y, n)) continue;
                float r = std::hypot(rel_x,rel_y );

                float cosKR=ws.cosAngleKR(rel_x, rel_y, n);

                if(r==0.0) continue;
                real_sum+=std::sin(k*r)/r*cosKR;
                imag_sum+=std::cos(k*r)/r*cosKR;
            }
            
            float local_amplitude=std::hypot(real_sum, imag_sum);

            abs_average+=std::abs(local_amplitude);
                
            grid_amplitude[pos_point]=local_amplitude;
            grid_phase[pos_point]=std::atan2(real_sum,imag_sum);
                  
        }
            
    }
    float scaling_wave_factor=screen_height*screen_width/abs_average*0.7;
    for(unsigned int i=0;i<screen_height*screen_width;i++){
        grid_amplitude[i]=grid_amplitude[i]*scaling_wave_factor;
    }
}

// Calculates the field values for each pixel of the screen.
void GridWave::getWaveValues(std::vector<unsigned char> & y_values,float time)
{
    for(unsigned int i=0;i<screen_height*screen_width;i++){
        float value=grid_amplitude[i]*sin(grid_phase[i]-w*time);
        if(value >1) value=1;
        else if(value<-1) value=-1;
        y_values[i]=(unsigned char)(127*value+127);
    
    }
}

// returns the wave lenght
int GridWave::getWaveLength(){
    return wavelength;
}
  
//returns the period
float GridWave::getWavePeriod(){
    return period;
}

// returns the number of sources that produce the waves
int GridWave::getNSources(){
    return ws.getNSources();
}

// returns the distances between the sources
int GridWave::getDistanceSources(){
    if(ws.getNSources()!=1)
        return ws.x(1) - ws.x(0);
    else
        return 0;
}

int GridWave::getSourcesWidth(){
    return round(ws.getSourcesWidth());
}




