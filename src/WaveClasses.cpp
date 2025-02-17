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
#include "GW_images.h"

#define PI 3.14159265358979323846

#define FLOAT double

/*
 * The constructor puts the value at 0.
 */
GFParameters::GFParameters(){
    resetValues();
}

/*
 * Resets the value to 0.
 */
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

/*
 * Checks if the object variables has been initialized.
 */
bool GFParameters::checkEmpty(){
    if(wavelength==0 && speed == 0 && slits_distance==0 && amplitude_func==0 && slit_width==0 &&initial_wave_form==0 && n_sources==0){
        return true;
    }
    else{
        return false;
    }
}

/*
 * Makes sure the variable are correctly adjusted (positive numbers for example).
 */
bool GFParameters::checkValuesSetCorrectly()
{
    if(wavelength<=0|| speed<=0 || slits_distance<0 || slit_width<=0 || n_sources<1 || view_scale_factor<0)
        return false;
    else
        return true;
        
}

/*
 * Gives the parameters that are used to build the grid.
 *
 * return : A GFParameters object
 */
GFParameters GridField::getParameters(){
        
    return parameters;

}

/*
 * Overload the operator == to check if two GFParameters object have the same values.
 *
 * return : True if the parameters are identical.
 */
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

/*
 * Prints on the screen the parameters of the object. Useful for debugging.
 */

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
    if(initial_wave_form == waveForm::circular )
        std::cout <<"Circular wave" << std::endl;
    if(initial_wave_form == waveForm::doubleSlit )
        std::cout <<"Double slit" << std::endl;
    
    std::cout << "Show intensity : " << show_intensity << std::endl;
    
    std::cout << "Scale factor the the visual representation: " << view_scale_factor<< std::endl;

}

/*
 * Checks if the precalculations will have to be redone by comparing the old parameters with the new.
 * Some parameters change do not requiere to recalculate the grid.
 
 * return : true if the precalculations in a GridField object have to be done again.
 */

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

/*
 * Sets the parameters to some default values.
 */
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


/*
 * Default constructor for WaveSources objects.
 */
    
WaveSources::WaveSources(){
    window_width=0;
    window_height=0;
    sources_coord.push_back({0.0,0.0});
    amplitude_func=amplitudeForm::one;
}

/*
 * Sets the size of the output screen where the result of the wavelets superposition will
 * be drawn.
 *
 * input_width : The width of the main window.
 * input_height : The height of the main window.
 */
void WaveSources::setScreenSize(unsigned int input_width, unsigned int input_height ){
    window_width=input_width;
    window_height=input_height;
}

/*
 * Sets how the amplitude of the wavelets will be calculated (dependency on the direction or not).
 *
 * input_amplitufe_func : An integer reprensenting how the wavelets amplitude will be treated ( should be set with the namespace waveForm).
 */
void WaveSources::setAmplitudeForm(unsigned int input_amplitude_func)
{
    amplitude_func=input_amplitude_func;
}

/*
 * Gets the initial wave type.
 *
 * return : An integer representing the initial wave type (see waveForm namescape).
 */
unsigned int WaveSources::getInitialWaveForm()
{
   return initial_wave_form;
}

/*
 * Reinitializes the wavelets positions considering the initial wave is a plane wave.
 *
 * n_sources : The number of sources for the initial slit.
 * input_slit_width : The slit width.
 */
void WaveSources::reinitializePlane( unsigned int n_sources,FLOAT input_slit_width){
    initial_wave_form=waveForm::plane;
    slits_distance=0;
    
    if(window_width==0 || window_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    slit_width=input_slit_width;
    
    FLOAT distance;
    if(n_sources>1)
        distance=slit_width/FLOAT(n_sources-1);
    else{
        distance=0;
        slit_width=0;
    }
    sources_coord.clear();
    direction_wave.clear();
    
    FLOAT start_x_coord=(FLOAT(window_width)-slit_width)/2.0;
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord+FLOAT(n)*distance;
        FLOAT y_coord=FLOAT(window_height)-10;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
    }
}

/*
 * Reinitializes the wavelets positions considering the initial wave is a circular  wave.
 *
 * n_sources : The nomber of sources for the circle.
 * circonference : The circonference of the initial circular wave.
 */
void WaveSources::reinitializeCircular( unsigned int n_sources,FLOAT circonference){
    initial_wave_form=waveForm::plane;
    
    slits_distance=0;
    
    FLOAT radius=circonference/2.0/PI;
    if(window_width==0 || window_height==0)
    {
        std::cerr << "The screen size has not been defined in WaveSources."<< std::endl;
        exit(0);
    }
    sources_coord.clear();
    direction_wave.clear();
    
    slit_width=circonference;

    
    FLOAT start_x_coord=FLOAT(window_width)/2;
    FLOAT start_y_coord=FLOAT(window_height-10)-radius;
    
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT angle=2*n*PI/FLOAT(n_sources)-PI/2;
        FLOAT rel_x=radius*std::cos(angle);
        FLOAT rel_y=radius*std::sin(angle);
        FLOAT x_coord=start_x_coord+rel_x;
        FLOAT y_coord=start_y_coord+rel_y;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({rel_x/radius,rel_y/radius});
    }
}

/*
 * Reinitializes the wavelets positions considering the initial wave is from a double slit.
 *
 * n_sources : The number of sources per slit.
 * input_slit_width : The width of each slit.
 * input_slits_distance : The distance between the slits.
 */
void WaveSources::reinitializeDoubleSlit( unsigned int n_sources,FLOAT input_slit_width, FLOAT input_slits_distance){
    initial_wave_form=waveForm::doubleSlit;
    
    if(window_width==0 || window_height==0)
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
        distance_sources=slit_width/FLOAT(n_sources-1);
    else{
        distance_sources=0;
        slit_width=0;
    }
    
    sources_coord.clear();
    direction_wave.clear();
    
    FLOAT start_x_coord1=(FLOAT(window_width)-slit_width-slits_distance)/2.0;
    FLOAT start_x_coord2=start_x_coord1+slits_distance;

    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord1+FLOAT(n)*distance_sources;
        FLOAT y_coord=FLOAT(window_height)-10.0;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
        
    }
    
    for(unsigned int n=0;n<n_sources;n++){
        FLOAT x_coord=start_x_coord2+FLOAT(n)*distance_sources;
        FLOAT y_coord=FLOAT(window_height)-10;
        sources_coord.push_back({x_coord,y_coord});
        direction_wave.push_back({0.0,-1.0});
        
    }
}

/*
 * Gives the x coordinate of the source n.
 *
 * n : The index of the source to access to.
 *
 * return : The x coordinate of source n.
 */
FLOAT WaveSources::x(int n){
    return sources_coord[n].first;
}

/*
 * Gives the y coordinate of the source n.
 *
 * n : The index of the source to access to.
 * return : The y coordinate of source n.
 */
FLOAT WaveSources::y(int n){
    return sources_coord[n].second;
}

/*
 * Prints the coordinate of each source with its index.
 * Mostly used for debugging purposes.
 */
void WaveSources::print(){
    for(unsigned int n=0;n<sources_coord.size();n++){
        std::cout <<"Source #" << n << " :" << x(n) << " , " << y(n) << std::endl;
    }
}

/*
 * Gives the number of sources in the WaveSources object
 *
 * return : The number of sources.
 */
unsigned int WaveSources::getNSources(){
    return (unsigned int)sources_coord.size();
}

/*
 * Checks if the wavelet n is going in the directiion of the coordinate x,y or not.
 * This is used to make sure that the wavelets do not go backward.
 *
 * x : The horizontal coordinate to verify.
 * y : The vertical coordinate to verify.
 * n : The index of the wavelet being checked.
 *
 * return : true if the wavelet nis going forward
 */
bool WaveSources::isRightDirection(int x, int y, unsigned int n){
    if(x*direction_wave[n].first +y*direction_wave[n].second >0) return true;
    else return false;
}

/*
 * Gives the amplitude modification of a wavelet. If amplitude_func is set to amplitudeForm::costheta
 * the program will make the wavelet amplitude be muliplited by cos(theta), where theta is the angle
 * between r (the position vector where the field is calculated relative to the coordinate of the wavelet)
 * and k (the k vector of the initial wave where the wavelet is ).
 *
 * x : The horizontal coordinate relative to the wavelet source.
 * y : The vertical coordinate relative to the wavelet source.
 * n : The index of the wavelet.
 *
 * return : 1 if no thete dependency, otherwise cos(k*r)
 */
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



 
/*
 * Constructor to initialize the GridField object. It will do all necessary precalculations
 * to make the program ready to calculte the field values of the combiniation of the wavelets.
 *
 * input_window_width : The width of the main window in which the drawing will be done.
 * input_window_height : The width of the main window in which the drawing will be done.
 * input_params : A GFParameters object containing all the parameters necessary for the calculations
 */


 GridField::GridField(int input_window_width, int input_window_height, GFParameters input_params){
    window_width=input_window_width;
    window_height=input_window_height;
    ws.setScreenSize(window_width,window_height);
    
    grid_phase.resize(window_width*window_height,0);
    grid_amplitude.resize(window_width*window_height,0);
    
    
    initializeParamAndGrid(input_params);
}


/*
 * (Re)initializes the object with new parameters. If necessary, all precalculations will
 * be done to make the object ready to calculate the time dependency of the resulting wave.
 *
 * The new parameters will be compared with the old ones to check if new precalculations are necessary.
 *
 * new_parameters : the new parameters
 */
void GridField::initializeParamAndGrid(GFParameters new_parameters){
    
    if(new_parameters.checkEmpty()|| new_parameters==parameters)
    {
        return;
    }
    
    period=new_parameters.wavelength/new_parameters.speed;
    w= 2*PI/period;
    
    if(!new_parameters.checkIfGridChange(parameters))
    {
        parameters=new_parameters;
        return;
    }
    
    parameters=new_parameters;

    k=2*PI/parameters.wavelength;
    
    ws.setAmplitudeForm(parameters.amplitude_func);
    

    if(new_parameters.initial_wave_form==waveForm::plane){
        ws.reinitializePlane(parameters.n_sources, parameters.slit_width);
    }
    else if(parameters.initial_wave_form==waveForm::circular){
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
/*
 * This method is used to test the behaviour of the resulting wave.
 */

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

/*
 * Initializes the data vectors for the phases and the amplitudes of the spatial part of the sum of the wavelets.
 * What is calculated is the real and imaginary parts of  -iΣ e^(ikx)/r. The real part represents the local amplitude
 * of the resulting wave and the imaginary part represents the local phase of the resulting wave. This term needs to
 * be multiplied by e^(-iwt) to get the time evolution of the resulting wave.
 *
 * This part of the code makes sure the wavelets are going forward and can consider a theta dependency of the
 * wavelets amplitude.
 */
void GridField::initialize_grid(){
    if(grid_phase.size()!=window_width*window_height) grid_phase.resize(window_width*window_height,0);
    if(grid_amplitude.size()!=window_width*window_height) grid_amplitude.resize(window_width*window_height,0);


    FLOAT max_amplitude_value=0;
    
    for( int j=0;j<(int)window_height;j++){
        for( int i=0;i<(int)window_width;i++){

            unsigned int pos_point=(unsigned int)(j*window_width+i);
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

    // The next lines are executed to make sure the field values do not exceed 1 too often,
    // so the color scale is showing most of the data.
    std::vector<unsigned int> count(101,0);
    for(unsigned int i=0;i<window_height*window_width;i++){
        count[int(100*grid_amplitude[i]/max_amplitude_value)]++;
    }
    
    FLOAT cummulative=0;
    for(unsigned int i=100;i>=0;i--){
        cummulative += FLOAT(count[i])/FLOAT(window_height*window_width) ;
        if (cummulative>0.02){
            max_amplitude_value=max_amplitude_value*FLOAT(i+1)/100.0;
            
            break;
        }
    }

    FLOAT scaling_wave_factor =1.0/max_amplitude_value;
    
    for(unsigned int i=0;i<window_height*window_width;i++){
        grid_amplitude[i]=grid_amplitude[i]*scaling_wave_factor;
    }
}



/*
 * Updates the vector containing pixels RGB values of the resulting wave to plot on the main window.
 *
 * pixels_values : A 4*height*width vector in which the pixels value will be written.
 */
 void GridField::getPixelsValues(std::vector<std::uint8_t> & pixels_values,FLOAT time)
{
    
    FLOAT r_positive=RGB_amplitude_max[0];
    FLOAT g_positive=RGB_amplitude_max[1];
    FLOAT b_positive=RGB_amplitude_max[2];
    FLOAT r_negative=RGB_amplitude_min[0];
    FLOAT g_negative=RGB_amplitude_min[1];
    FLOAT b_negative=RGB_amplitude_min[2];
    
    FLOAT r_zero=RGB_amplitude_zero[0];
    FLOAT g_zero=RGB_amplitude_zero[2];
    FLOAT b_zero=RGB_amplitude_zero[2];

    
    FLOAT adjusted_scale_factor=(std::exp(1.5*parameters.view_scale_factor)-1)/(std::exp(1.5)-1);

    adjusted_scale_factor=std::exp((adjusted_scale_factor))-1;
    if(parameters.show_intensity==false){
        for(unsigned int i=0;i<window_height*window_width;i++){
            FLOAT value=grid_amplitude[i]*sin(grid_phase[i]-w*time)*adjusted_scale_factor;
            if(value >1.0) value=1.0;
            else if(value<-1) value=-1;
            
            
            if(value>=0){
                pixels_values[i*4+0]=(unsigned char) (r_positive*value+r_zero*(1-value));
                pixels_values[i*4+1]=(unsigned char) (g_positive*value+g_zero*(1-value));
                pixels_values[i*4+2]=(unsigned char) (b_positive*value+b_zero*(1-value));
                pixels_values[i*4+3]=(unsigned char) 255;
            }
            else{
                pixels_values[i*4+0]=(unsigned char) (r_negative*abs(value)+r_zero*(1-abs(value)));
                pixels_values[i*4+1]=(unsigned char) (g_negative*abs(value)+g_zero*(1-abs(value)));
                pixels_values[i*4+2]=(unsigned char) (b_negative*abs(value)+b_zero*(1-abs(value)));
                pixels_values[i*4+3]=(unsigned char) (255);
            }

        }
    }
    else
    {
        for(unsigned int i=0;i<window_height*window_width;i++){
            FLOAT value=grid_amplitude[i]*grid_amplitude[i]*adjusted_scale_factor;
            if(value>1) value=1;
            pixels_values[i*4]=(unsigned char)(value*255);
            pixels_values[i*4+1]=(unsigned char)(value*255);
            pixels_values[i*4+2]=(unsigned char)(value*255);
            pixels_values[i*4+3]=255;
        }
    }
}


/*
 * Returns the number of sources (wavelets).
 */

unsigned int GridField::getNSources(){
    return ws.getNSources();
}

/*
 * Adds on the RGB pixels values of the resulting wave the position of the sources.
 */
void GridField::drawSourcesScreen(std::vector<std::uint8_t> &pix_table){
    if(ws.getNSources()<=0) {std::cerr << "No sources in WaveSources" << std::endl; exit(0);}
    
    for(unsigned int n=0;n<ws.getNSources();n++){
        int x=std::round(ws.x(n));
        int y=std::round(ws.y(n));
        for(int j=y-2; j<=y+2;j++ ){
            if(j<0 || j>= (int)window_height) continue;
            for(int i=x-2; i<=x+2;i++ ){
                if(i<0 || i>= (int)window_width) continue;

                unsigned int pointer_pos=(j*window_width+i)*4;
                pix_table[pointer_pos+0]= RGB_sources[0];
                pix_table[pointer_pos+1]= RGB_sources[1];
                pix_table[pointer_pos+2]= RGB_sources[2];
                pix_table[pointer_pos+3]= 255;
            }
        }
    }
}

/*
 * Adds an image on the main window pixels values.
 *
 * pix_table : The table containing the RGB values of the pixels that will be drawn on the main window.
 * x_start : The x coordinate of the starting position of the image.
 * y_start : The y coordinate of the starting position of the image.
 * image_vector : A height*width vector containing the values (from 0 to 255) of the image to add.
 * rgb_text : The RGB color of the image to be draw.
 */
void GridField::addImage(std::vector<std::uint8_t> &pix_table, unsigned int x_start, unsigned int y_start, std::vector<std::vector<unsigned char>>  image_vector, std::vector<unsigned char> rgb_text){
    
    if(x_start<0) x_start=0;
    if(y_start<0) y_start=0;
    
    if(x_start+image_vector[0].size()>window_width) x_start=x_start-(unsigned int)image_vector[0].size()-1;
    if(y_start+image_vector.size()>window_height) y_start=y_start-(unsigned int)image_vector.size()-1;
    
    for(unsigned int j=0; j<image_vector.size();j++){
        unsigned int y_coord=y_start+j;
        for(unsigned int i=0; i<image_vector[0].size();i++){
            unsigned int x_coord=x_start+i;
            unsigned int pointer_pos=(y_coord*window_width+x_coord)*4;
            unsigned char value_text=image_vector[j][i];
            pix_table[pointer_pos+0]= (255-value_text)*rgb_text[0]/255 + value_text*pix_table[pointer_pos+0]/255;
            pix_table[pointer_pos+1]= (255-value_text)*rgb_text[1]/255 + value_text*pix_table[pointer_pos+1]/255;
            pix_table[pointer_pos+2]= (255-value_text)*rgb_text[2]/255 + value_text*pix_table[pointer_pos+2]/255;
            pix_table[pointer_pos+3]= 255;
        }
    }
}

/*
 * Adds the color scale associated to the resulting wave amplitude/intensity for the main window.
 */
void GridField::drawScaleScreen(std::vector<std::uint8_t> &pix_table){
    if(parameters.show_intensity==false){
        drawScaleScreenAmplitude(pix_table);
    }
    else{
        drawScaleScreenIntensity(pix_table);
    }
}

/*
 * Adds the color scale associated to the resulting wave amplitude for the main window.
 */
void GridField::drawScaleScreenAmplitude(std::vector<std::uint8_t> &pix_table){
    unsigned int x_start=window_width-80;
    unsigned int y_start=20;
    
    unsigned int scale_width=30;
    unsigned int scale_height=150;
    
    unsigned int text_x_pos=x_start-(unsigned int)GridWaveImages::max_image_data[0].size()-5;
    unsigned int text_y_pos=y_start-(unsigned int)GridWaveImages::max_image_data.size()/2;
    addImage(pix_table,text_x_pos,text_y_pos,GridWaveImages::max_image_data,RGB_amplitude_text);

    text_x_pos=x_start-(unsigned int)GridWaveImages::zero_image_data[0].size()-5;
    text_y_pos=y_start-(unsigned int)GridWaveImages::zero_image_data.size()/2 +scale_height/2;
    addImage(pix_table,text_x_pos,text_y_pos,GridWaveImages::zero_image_data,RGB_amplitude_text);
    
    text_x_pos=x_start-(unsigned int)GridWaveImages::minus_max_image_data[0].size()-5;
    text_y_pos=y_start-(unsigned int)GridWaveImages::minus_max_image_data.size()/2+scale_height;
    addImage(pix_table,text_x_pos,text_y_pos,GridWaveImages::minus_max_image_data,RGB_amplitude_text);
    
    
    unsigned int border_size=2;
    unsigned char r_value, g_value, b_value;
    
    for(unsigned int j=0; j<scale_height;j++){
        unsigned int y_coord=y_start+j;
        if(j<=scale_height/2){
            r_value=RGB_amplitude_max[0]*(scale_height-2*j)/scale_height + RGB_amplitude_zero[0]*2*j/scale_height;
            g_value=RGB_amplitude_max[1]*(scale_height-2*j)/scale_height + RGB_amplitude_zero[1]*2*j/scale_height;
            b_value=RGB_amplitude_max[2]*(scale_height-2*j)/scale_height + RGB_amplitude_zero[2]*2*j/scale_height;
        }
        else{
            r_value=RGB_amplitude_zero[0]*2*(scale_height-j)/scale_height + RGB_amplitude_min[0]*(2*j-scale_height)/scale_height;
            g_value=RGB_amplitude_zero[1]*2*(scale_height-j)/scale_height + RGB_amplitude_min[1]*(2*j-scale_height)/scale_height;
            b_value=RGB_amplitude_zero[2]*2*(scale_height-j)/scale_height + RGB_amplitude_min[2]*(2*j-scale_height)/scale_height;
        }
                
        for(unsigned int i=0; i<scale_width;i++){
            unsigned int x_coord=x_start+i;
            unsigned int pointer_pos=(y_coord*window_width+x_coord)*4;
            if(i<border_size || i>scale_width-border_size-1 || j<border_size || j>scale_height-border_size-1 ){
                pix_table[pointer_pos+0]= RGB_amplitude_text[0];
                pix_table[pointer_pos+1]= RGB_amplitude_text[1];
                pix_table[pointer_pos+2]= RGB_amplitude_text[2];
                pix_table[pointer_pos+3]= 255;
            }
            else if((j==scale_height/2) && (i<6))
            {
                pix_table[pointer_pos+0]= RGB_amplitude_text[0];
                pix_table[pointer_pos+1]= RGB_amplitude_text[1];
                pix_table[pointer_pos+2]= RGB_amplitude_text[2];
                pix_table[pointer_pos+3]= 255;
            }
            else{
                pix_table[pointer_pos+0]= r_value;
                pix_table[pointer_pos+1]= g_value;
                pix_table[pointer_pos+2]= b_value;
                pix_table[pointer_pos+3]= 255;
            }

        }

    }
}

/*
 * Adds the color scale associated to the resulting wave instensity for the main window.
 */
void GridField::drawScaleScreenIntensity(std::vector<std::uint8_t> &pix_table){
    unsigned int x_start=window_width-80;
    unsigned int y_start=20;
    
    unsigned int scale_width=30;
    unsigned int scale_height=150;
    
    unsigned int text_x_pos=x_start-(unsigned int)GridWaveImages::max_image_data[0].size()-5;
    unsigned int text_y_pos=y_start-(unsigned int)GridWaveImages::max_image_data.size()/2;
    addImage(pix_table,text_x_pos,text_y_pos,GridWaveImages::max_image_data,RGB_intensity_text);

    text_x_pos=x_start-(unsigned int)GridWaveImages::zero_image_data[0].size()-5;
    text_y_pos=y_start-(unsigned int)GridWaveImages::zero_image_data.size()/2 +scale_height;
    addImage(pix_table,text_x_pos,text_y_pos,GridWaveImages::zero_image_data,RGB_intensity_text);
    
    unsigned int border_size=2;
    unsigned char r_value, g_value, b_value;
    
    for(unsigned int j=0; j<scale_height;j++){
        unsigned int y_coord=y_start+j;
        r_value=RGB_intensity_max[0]*(scale_height-j)/scale_height + RGB_intensity_zero[0]*j/scale_height;
        g_value=RGB_intensity_max[1]*(scale_height-j)/scale_height + RGB_intensity_zero[1]*j/scale_height;
        b_value=RGB_intensity_max[2]*(scale_height-j)/scale_height + RGB_intensity_zero[2]*j/scale_height;
        
        for(unsigned int i=0; i<scale_width;i++){
            unsigned int x_coord=x_start+i;
            unsigned int pointer_pos=(y_coord*window_width+x_coord)*4;
            if(i<border_size || i>scale_width-border_size-1 || j<border_size || j>scale_height-border_size-1 ){
                pix_table[pointer_pos+0]= RGB_intensity_text[0];
                pix_table[pointer_pos+1]= RGB_intensity_text[1];
                pix_table[pointer_pos+2]= RGB_intensity_text[2];
                pix_table[pointer_pos+3]= 255;
            }
            else{
                pix_table[pointer_pos+0]= r_value;
                pix_table[pointer_pos+1]= g_value;
                pix_table[pointer_pos+2]= b_value;
                pix_table[pointer_pos+3]= 255;
            }
            
        }
                
    }
}
