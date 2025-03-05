#ifndef GUICLASS_H
#define GUICLASS_H

#include <vector>

#ifndef PI
#define PI 3.14159265358979323846
#endif


template <typename T> T averageCyclic(std::vector<T> data, std::vector<T> kr, unsigned int max_size){

    double  period_osc;
    
    unsigned int adjusted_max_size;
    
    auto dim = data.size();
    
    double average=0;
    
    if(max_size > data.size()) adjusted_max_size=(unsigned int)(data.size());
    else adjusted_max_size=max_size;
    
    double min_decimal=1.0;
    unsigned int index_min_decimal=0;


    
    for(unsigned int j=2; j<adjusted_max_size; j++){

        period_osc=2*PI/(kr[dim-j/2-1]-kr[dim-j/2-2]);
        double n=period_osc*std::round(j/period_osc);
            
        double decimal= n-int(n);

        if(decimal>0.5) decimal =  1-decimal;
            
        if(decimal < min_decimal){
            min_decimal=decimal;
            index_min_decimal=j;
        }
        
        for( int j = -index_min_decimal+1 ; j<=0; j++)
        {
            average+=data[dim-1+j]/double(index_min_decimal);
        }
    }
    return average;
}





#endif
