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

#define FLOAT double


GFParameters::GFParameters(){
    resetValues();
}

void GFParameters::resetValues(){
    wavelength=0;
    speed=0;
    slits_distance=0;
    amplitude_func=0;
    slit_width=0;
    initial_wave_form=0;
    n_sources=0;
    show_intensity=false;
    view_scale_factor=1;
}



bool GFParameters::checkEmpty(){
    if(wavelength==0 && speed == 0 && slits_distance==0 && amplitude_func==0 && slit_width==0 &&initial_wave_form==0 && n_sources==0){
        return true;
    }
    else{
        return false;
    }
}

bool GFParameters::checkValuesSetCorrectly()
{
    if(wavelength<=0|| speed<=0 || slits_distance<0 || slit_width<=0 || n_sources<1 || view_scale_factor<0)
        return false;
    else
        return true;
        
}

bool GFParameters::operator==(const GFParameters& gfp_compare) const
{
    if(   wavelength==gfp_compare.wavelength
       && speed==gfp_compare.speed
       && slits_distance==gfp_compare.slits_distance
       && amplitude_func==gfp_compare.amplitude_func
       && slit_width==gfp_compare.slit_width
       && initial_wave_form==gfp_compare.initial_wave_form
       && n_sources==gfp_compare.n_sources
       && show_intensity==gfp_compare.show_intensity
       && view_scale_factor==gfp_compare.view_scale_factor)
        return true;
    else
        return false;
        
}

void GFParameters::printParameters()
{
    std::cout << "Wavelenght : " << wavelength << std::endl;
    std::cout << "Wave speed : " << speed << std::endl;
    std::cout << "Distance between slits : " << slits_distance << std::endl;
    std::cout << "Source waves amplitude function : ";
    if(amplitude_func == amplitudeForm::one  )
        std::cout << "No k/θ dependancy"<<std::endl;
    else if(amplitude_func == amplitudeForm::costheta)
        std::cout << "Source have a cos(θ) dependancy"<<std::endl;
    else
        std::cout << "Not correctly defined"<<std::endl;

    std::cout << "Slit width : " << slit_width << std::endl;
    std::cout << "Number of sources : " << n_sources << std::endl;
    
    std::cout << "Initial wave form= : ";
    if(initial_wave_form == waveForm::plane )
        std::cout <<"Plane wave" << std::endl;
    if(initial_wave_form == waveForm::spherical )
        std::cout <<"Spherical wave" << std::endl;
    if(initial_wave_form == waveForm::doubleSlit )
        std::cout <<"Double slit" << std::endl;
    
    std::cout << "Show intensity : " << show_intensity << std::endl;
    
    std::cout << "Scale factor the the visual representation: " << view_scale_factor<< std::endl;


}


bool GFParameters::checkIfGridChange(const GFParameters& gfp_compare){

    if(wavelength!=gfp_compare.wavelength
       || amplitude_func!=gfp_compare.amplitude_func
       || slit_width!=gfp_compare.slit_width
       || initial_wave_form!=gfp_compare.initial_wave_form
       || n_sources!=gfp_compare.n_sources
       || initial_wave_form!=gfp_compare.initial_wave_form)
    {
        return true;
    }
    else{
        if(initial_wave_form==waveForm::doubleSlit){
            if(slits_distance!=gfp_compare.slits_distance)
                return true;
            else
                return false;
        }
        else
            return false;
    }

}

void GFParameters::setDefaultParameters(){
    wavelength=50;
    speed=40;
    slits_distance=200;
    amplitude_func=amplitudeForm::costheta;
    slit_width=800;
    initial_wave_form=waveForm::plane;
    n_sources=40;
    show_intensity=false;
    view_scale_factor=1;
}

//**************************************************
//
// WaveSources class is used to manage the sources that produce the waves.
//
//**************************************************

    
WaveSources::WaveSources(){
    screen_width=0;
    screen_height=0;
    sources_coord.push_back({0.0,0.0});
    amplitude_func=amplitudeForm::one;
}

// Sets the size of the screen
void WaveSources::setScreenSize(unsigned int input_width, unsigned int input_height ){
    screen_width=input_width;
    screen_height=input_height;
}

void WaveSources::setAmplitudeForm(unsigned int input_amplitude_func)
{
    amplitude_func=input_amplitude_func;
}

unsigned int WaveSources::getAmplitudeForm()
{
   return amplitude_func;
}

unsigned int WaveSources::getInitialWaveForm()
{
   return initial_wave_form;
}

// Initializes the sources by giving the number of sources and the distance between them
void WaveSources::reinitializePlane( unsigned int n_sources,unsigned int input_slit_width){
    initial_wave_form=waveForm::plane;
    slits_distance=0;
    
    if(screen_width==0 || screen_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    slit_width=input_slit_width;
    
    FLOAT distance;
    if(n_sources>1)
        distance=FLOAT(slit_width)/FLOAT(n_sources-1);
    else{
        distance=0;
        slit_width=0;
    }
    sources_coord.clear();
    direction_wave.clear();
    
    FLOAT start_x_coord=FLOAT(int(screen_width)-int(slit_width))/2.0;
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord+FLOAT(n)*distance;
        FLOAT y_coord=FLOAT(screen_height)-10;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
    }
}

void WaveSources::reinitializeCircular( unsigned int n_sources,unsigned int input_slit_width){
    initial_wave_form=waveForm::plane;
    
    slits_distance=0;
    
    FLOAT radius=FLOAT(input_slit_width)/2.0/PI;
    if(screen_width==0 || screen_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    sources_coord.clear();
    direction_wave.clear();
    
    slit_width=input_slit_width;

    
    FLOAT start_x_coord=FLOAT(screen_width)/2;
    FLOAT start_y_coord=FLOAT(screen_height-10);
    
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT angle=2*n*PI/FLOAT(n_sources);
        FLOAT rel_x=radius*std::cos(angle);
        FLOAT rel_y=radius*std::sin(angle);
        FLOAT x_coord=start_x_coord+rel_x;
        FLOAT y_coord=start_y_coord+rel_y;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({rel_x/radius,rel_y/radius});
    }
}

void WaveSources::reinitializeDoubleSlit( unsigned int n_sources,unsigned int input_slit_width, unsigned int input_slits_distance){
    initial_wave_form=waveForm::doubleSlit;
    
    if(screen_width==0 || screen_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    
    slit_width=input_slit_width;
    slits_distance=input_slits_distance;
    
    if(slits_distance<=slit_width){
        reinitializePlane( n_sources*2, slit_width*2-slits_distance);
        return;
    }
    
    FLOAT distance_sources;
    
    if(n_sources>1)
        distance_sources=FLOAT(slit_width)/FLOAT(n_sources-1);
    else{
        distance_sources=0;
        slit_width=0;
    }
    
    sources_coord.clear();
    direction_wave.clear();
    
    FLOAT start_x_coord1=FLOAT(int(screen_width)-int(slit_width)-int(slits_distance))/2.0;
    FLOAT start_x_coord2=FLOAT(start_x_coord1+slits_distance);

    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord1+FLOAT(n)*distance_sources;
        FLOAT y_coord=FLOAT(screen_height)-10.0;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
        
    }
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord2+FLOAT(n)*distance_sources;
        FLOAT y_coord=FLOAT(screen_height)-10;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
        
    }
}


// Gives the x coordinate of the source n
FLOAT WaveSources::x(int n){
    return sources_coord[n].first;
}

// Gives the y coordinate of the source n
FLOAT WaveSources::y(int n){
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
    if(x*direction_wave[n].first +y*direction_wave[n].second >0) return true;
    else return false;
}

FLOAT WaveSources::cosAngleKR(FLOAT x, FLOAT y, int n){
    if(amplitude_func==amplitudeForm::costheta)
        return (x*direction_wave[n].first +y*direction_wave[n].second)/std::hypot(x,y);
    else if(amplitude_func==amplitudeForm::one)
        return 1.0;
    else{
        std::cerr << "Invalide amplitude_func (=" << amplitude_func << ") in WaveSources::cosAngleKR\n" << std::endl;
        exit(0);
    }
    return 1.0;
}

unsigned int WaveSources::getSourcesWidth(){
    return slit_width;
}

unsigned int WaveSources::getSlitsDistance(){
    return slits_distance;
}

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
 
// Initialize the grid for the calculations of the field values for each pixel of the screen
GridField::GridField(int input_screen_width, int input_screen_height, GFParameters input_params){
    screen_width=input_screen_width;
    screen_height=input_screen_height;
    ws.setScreenSize(screen_width,screen_height);
    
    grid_phase.resize(screen_width*screen_height,0);
    grid_amplitude.resize(screen_width*screen_height,0);
    
    
    initializeParamAndGrid(input_params);



}


void GridField::setAmplitudeForm(unsigned int input_amplitude_func)
{
    ws.setAmplitudeForm(input_amplitude_func);
}

unsigned int GridField::getAmplitudeForm()
{
   return ws.getAmplitudeForm();
}

// This method can be used to initialize the parameters of the GF or reinitialize
// them if they get change. The field grid values will be recalculated
void GridField::initializeParamAndGrid(GFParameters new_parameters){
    
    if(new_parameters.checkEmpty()|| new_parameters==parameters)
    {
        return;
    }
    

    FLOAT new_period=FLOAT(new_parameters.wavelength)/FLOAT(new_parameters.speed);
    w= 2*PI/new_period;
    
    if(!new_parameters.checkIfGridChange(parameters))
    {
        parameters=new_parameters;
        return;
    }
    
    parameters=new_parameters;

    
    k=2*PI/((FLOAT)parameters.wavelength);
    
    setAmplitudeForm(parameters.amplitude_func);
    

    if(new_parameters.initial_wave_form==waveForm::plane){
        ws.reinitializePlane(parameters.n_sources, parameters.slit_width);
    }
    else if(parameters.initial_wave_form==waveForm::spherical){
        ws.reinitializeCircular(parameters.n_sources, parameters.slit_width);
    }
    else if(parameters.initial_wave_form==waveForm::doubleSlit){
        ws.reinitializeDoubleSlit(parameters.n_sources, parameters.slit_width,parameters.slits_distance);
    }
    else{
        std::cerr << parameters.initial_wave_form << " type of wave is not compatible with the GridField class.\n";
    }
    
    initialize_grid();
    
   
}

// This method is used to test the behaviour of the wave amplitude
void GridField::testCalculations(){
    FLOAT sources_width=100000;

    FLOAT nnsources= sources_width*10;
    
    FLOAT kk=2*PI/10.0;
    

    unsigned int n_steps=50000;

    FLOAT y_step=1;

    std::vector<float> values(n_steps);
    
    FLOAT average=0;
    
    for(unsigned int i=0;i<n_steps;i++){
        FLOAT im_part=0, re_part=0;
        FLOAT x=0;
        FLOAT y=FLOAT(i)*y_step+10;
        for(FLOAT sx = -sources_width/2;sx< sources_width/2;sx+=sources_width/nnsources){
            float r=std::hypot(x-sx,y);
            re_part+=cos(kk*r)/sqrt(r);
            im_part+=sin(kk*r)/sqrt(r);
        }
        values[i]=std::hypot(re_part, im_part);
        average+=values[i]/n_steps;
    }
    
    for(unsigned int i=0;i<n_steps;i++){
        FLOAT y=FLOAT(i)*y_step+10;
        std::cout << y << "\t" << values[i]/average << std::endl;

    }

    exit(0);
    
}

// Initialize the arrays for the phases and the amplitudes of the spatial part of the sum of the waves.
void GridField::initialize_grid(){
    if(grid_phase.size()!=screen_width*screen_height) grid_phase.resize(screen_width*screen_height,0);
    if(grid_amplitude.size()!=screen_width*screen_height) grid_amplitude.resize(screen_width*screen_height,0);


    FLOAT max_amplitude_value=0;
    
    for( int j=0;j<(int)screen_height;j++){
        for( int i=0;i<(int)screen_width;i++){

            unsigned int pos_point=(unsigned int)(j*screen_width+i);
            FLOAT real_sum=0;
            FLOAT imag_sum=0;
                        
            for(unsigned int n=0;n<ws.getNSources();n++){
                
                FLOAT rel_x=FLOAT(i)-ws.x(n);
                FLOAT rel_y=FLOAT(j)-ws.y(n);
                if(!ws.isRightDirection(rel_x, rel_y, n)) continue;
                FLOAT r = std::hypot(rel_x,rel_y );
                if(r==0.0) continue;
                
                FLOAT sqrt_r= sqrt(r);
                
                FLOAT cosKR=ws.cosAngleKR(rel_x, rel_y, n);

                //for a 2D  wave, the amplitude of wavelets depends on 1/sqrt(r)
                real_sum+=std::sin(k*r)/sqrt_r*cosKR;
                imag_sum+=std::cos(k*r)/sqrt_r*cosKR;
            }
            
            FLOAT local_amplitude=std::hypot(-real_sum, -imag_sum);

            if(max_amplitude_value<local_amplitude) max_amplitude_value=local_amplitude;


            
            grid_amplitude[pos_point]=local_amplitude;
            grid_phase[pos_point]=std::atan2(real_sum,imag_sum);
                  
        }
            
    }

    // The next lines are executed to make sure the does not exceed 1 too often,
    // so the color scale is showing most of the data.
    std::vector<unsigned int> count(101,0);
    for(unsigned int i=0;i<screen_height*screen_width;i++){
        count[int(100*grid_amplitude[i]/max_amplitude_value)]++;
    }
    
    FLOAT cummulative=0;
    for(unsigned int i=100;i>=0;i--){
        cummulative += FLOAT(count[i])/FLOAT(screen_height*screen_width) ;
        if (cummulative>0.02){
            max_amplitude_value=max_amplitude_value*FLOAT(i+1)/100.0;
            
            break;
        }
    }

    FLOAT scaling_wave_factor =1.0/max_amplitude_value;
    
    for(unsigned int i=0;i<screen_height*screen_width;i++){
        grid_amplitude[i]=grid_amplitude[i]*scaling_wave_factor;
    }
}



// Calculates the field values for each pixel of the screen.
void GridField::getPixelsValues(std::vector<std::uint8_t> & pixels_values,FLOAT time)
{
    
    int r_positive=23,g_positive=116,b_positive=219;
    int r_negative=255,g_negative=124,b_negative=20;

    
    FLOAT adjusted_scale_factor=(std::exp(1.5*parameters.view_scale_factor)-1)/(std::exp(1.5)-1);

    adjusted_scale_factor=std::exp((adjusted_scale_factor))-1;
    if(parameters.show_intensity==false){
        for(unsigned int i=0;i<screen_height*screen_width;i++){
            FLOAT value=grid_amplitude[i]*sin(grid_phase[i]-w*time)*adjusted_scale_factor;
            if(value >1.0) value=1.0;
            else if(value<-1) value=-1;
            
            
            if(value>=0){
                pixels_values[i*4+0]=(unsigned char) r_positive+(255-r_positive)*(1-value);
                pixels_values[i*4+1]=(unsigned char) g_positive+(255-g_positive)*(1-value);
                pixels_values[i*4+2]=(unsigned char) b_positive+(255-b_positive)*(1-value);
                pixels_values[i*4+3]=(unsigned char) 255;
            }
            else{
                pixels_values[i*4+0]=(unsigned char)(r_negative+(255-r_negative)*(1+value));
                pixels_values[i*4+1]=(unsigned char)(g_negative+(255-g_negative)*(1+value));
                pixels_values[i*4+2]=(unsigned char)(b_negative+(255-b_negative)*(1+value));
                pixels_values[i*4+3]=(unsigned char) (255);
            }

        }
    }
    else
    {
        for(unsigned int i=0;i<screen_height*screen_width;i++){
            FLOAT value=grid_amplitude[i]*grid_amplitude[i]*adjusted_scale_factor;
            if(value>1) value=1;
            pixels_values[i*4]=(unsigned char)(value*255);
            pixels_values[i*4+1]=(unsigned char)(value*255);
            pixels_values[i*4+2]=(unsigned char)(value*255);
            pixels_values[i*4+3]=255;


        }


    }
}

// returns the wave lenght
unsigned  int GridField::getWaveLength(){
    return parameters.wavelength;
}
  
//returns the period
FLOAT GridField::getWavePeriod(){
    return period;
}

// returns the number of sources that produce the waves
unsigned int GridField::getNSources(){
    return ws.getNSources();
}

// returns the distances between the sources
unsigned int GridField::getDistanceSources(){
    if(ws.getNSources()!=1)
        return ws.x(1) - ws.x(0);
    else
        return 0;
}

unsigned int GridField::getSourcesWidth(){
    return round(ws.getSourcesWidth());
}

unsigned int GridField::getSlitsDistance(){
    return ws.getSlitsDistance();
}

GFParameters GridField::getParameters(){
        
    return parameters;

}
