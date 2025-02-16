#ifndef GRIDWAVEIMAGES_H
#define GRIDWAVEIMAGES_H


namespace GridWaveImages{
    const int max_image_width = 41;
    const int max_image_height =16;
    std::vector<std::vector<unsigned char> >   max_image_data = {
        {250, 238, 238, 241, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 238, 238, 247, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 64, 245, 255, 255, 255, 255, 255, 255, 255, 255, 129, 0, 0, 194, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 189, 255, 255, 255, 255, 255, 255, 255, 226, 13, 0, 0, 194, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 96, 250, 255, 255, 255, 255, 255, 255, 163, 0, 0, 0, 194, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 0, 211, 255, 255, 255, 255, 255, 242, 46, 0, 0, 0, 194, 255, 255, 252, 235, 215, 203, 205, 223, 250, 255, 255, 255, 214, 200, 200, 242, 255, 255, 255, 254, 207, 200, 200, 248},
        {217, 0, 0, 86, 0, 122, 254, 255, 255, 255, 255, 185, 0, 79, 0, 0, 194, 255, 250, 98, 0, 0, 0, 0, 0, 71, 196, 255, 255, 222, 38, 0, 130, 251, 255, 255, 197, 0, 0, 167, 255},
        {217, 0, 0, 171, 50, 22, 227, 255, 255, 255, 251, 92, 0, 173, 0, 0, 194, 255, 249, 77, 61, 124, 145, 135, 42, 0, 0, 215, 255, 255, 185, 0, 0, 181, 255, 229, 53, 0, 118, 249, 255},
        {217, 0, 0, 194, 145, 0, 151, 255, 255, 255, 206, 0, 110, 215, 0, 0, 194, 255, 253, 234, 253, 255, 255, 255, 219, 0, 0, 162, 255, 255, 252, 135, 0, 34, 212, 109, 0, 59, 232, 255, 255},
        {217, 0, 0, 193, 218, 0, 50, 238, 255, 254, 130, 0, 189, 216, 0, 0, 194, 255, 255, 255, 255, 255, 255, 255, 254, 0, 0, 147, 255, 255, 255, 241, 81, 0, 22, 0, 0, 203, 255, 255, 255},
        {217, 0, 0, 193, 250, 92, 0, 179, 255, 227, 0, 34, 240, 216, 0, 0, 194, 255, 255, 229, 167, 109, 73, 81, 122, 0, 0, 147, 255, 255, 255, 255, 216, 34, 0, 0, 159, 255, 255, 255, 255},
        {217, 0, 0, 193, 255, 182, 0, 71, 246, 158, 0, 148, 255, 216, 0, 0, 194, 255, 219, 46, 0, 61, 110, 109, 90, 0, 0, 147, 255, 255, 255, 255, 234, 56, 0, 0, 187, 255, 255, 255, 255},
        {217, 0, 0, 193, 255, 238, 38, 0, 185, 50, 0, 211, 255, 216, 0, 0, 194, 255, 133, 0, 53, 236, 255, 254, 250, 0, 0, 147, 255, 255, 255, 250, 117, 0, 0, 0, 38, 223, 255, 255, 255},
        {217, 0, 0, 193, 255, 255, 133, 0, 22, 0, 77, 251, 255, 216, 0, 0, 194, 255, 104, 0, 69, 251, 255, 255, 244, 0, 0, 147, 255, 255, 255, 170, 0, 13, 180, 73, 0, 96, 245, 255, 255},
        {217, 0, 0, 193, 255, 255, 210, 0, 0, 0, 178, 255, 255, 216, 0, 0, 194, 255, 154, 0, 0, 139, 183, 161, 77, 0, 0, 147, 255, 255, 212, 22, 0, 149, 253, 209, 13, 0, 152, 254, 255},
        {217, 0, 0, 193, 255, 255, 249, 83, 0, 22, 229, 255, 255, 216, 0, 0, 194, 255, 235, 79, 0, 0, 0, 0, 81, 109, 0, 147, 255, 238, 71, 0, 93, 245, 255, 255, 167, 0, 0, 197, 255},
        {233, 170, 170, 221, 255, 255, 255, 202, 170, 181, 255, 255, 255, 233, 170, 170, 221, 255, 255, 243, 199, 176, 175, 203, 246, 240, 172, 200, 255, 205, 170, 170, 228, 255, 255, 255, 250, 177, 170, 177, 250}
    };

    const int min_image_width = 35;
    const int min_image_height =17;
    std::vector<std::vector<unsigned char> >   min_image_data = {
        {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {229, 143, 143, 163, 252, 255, 255, 255, 255, 255, 255, 255, 255, 193, 143, 143, 213, 255, 244, 142, 143, 205, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 221, 255, 255, 255, 255, 255, 255, 255, 246, 61, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 140, 255, 255, 255, 255, 255, 255, 255, 194, 0, 0, 0, 194, 255, 249, 203, 203, 229, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 0, 38, 235, 255, 255, 255, 255, 255, 253, 105, 0, 0, 0, 194, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
        {217, 0, 0, 34, 0, 167, 255, 255, 255, 255, 255, 213, 0, 28, 0, 0, 194, 255, 240, 42, 46, 182, 255, 255, 158, 46, 185, 242, 178, 99, 64, 118, 197, 252, 255},
        {217, 0, 0, 143, 13, 66, 244, 255, 255, 255, 255, 142, 0, 136, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 112, 81, 0, 0, 0, 0, 0, 180, 255},
        {217, 0, 0, 189, 99, 0, 192, 255, 255, 255, 233, 13, 56, 205, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 99, 202, 235, 213, 69, 0, 69, 247},
        {217, 0, 0, 194, 191, 0, 92, 251, 255, 255, 168, 0, 162, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 246, 255, 255, 255, 172, 0, 22, 234},
        {217, 0, 0, 193, 240, 53, 0, 212, 255, 246, 64, 0, 219, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {217, 0, 0, 193, 255, 144, 0, 122, 255, 190, 0, 101, 253, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {217, 0, 0, 193, 255, 218, 0, 28, 222, 105, 0, 187, 255, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {217, 0, 0, 193, 255, 251, 92, 0, 102, 0, 22, 237, 255, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {217, 0, 0, 193, 255, 255, 179, 0, 0, 0, 141, 255, 255, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {217, 0, 0, 193, 255, 255, 238, 38, 0, 0, 209, 255, 255, 216, 0, 0, 194, 255, 239, 0, 0, 180, 255, 255, 154, 0, 0, 253, 255, 255, 255, 186, 0, 0, 231},
        {216, 0, 0, 192, 255, 255, 254, 133, 0, 66, 249, 255, 255, 215, 0, 0, 193, 255, 239, 0, 0, 179, 255, 255, 153, 0, 0, 253, 255, 255, 255, 185, 0, 0, 230},
        {254, 251, 251, 253, 255, 255, 255, 252, 251, 251, 255, 255, 255, 254, 251, 251, 253, 255, 255, 251, 251, 252, 255, 255, 252, 251, 251, 255, 255, 255, 255, 253, 251, 251, 254}
    };

    const int zero_image_width = 13;
    const int zero_image_height =16;
    std::vector<std::vector<unsigned char> > zero_image_data = {
        {255, 255, 255, 255, 245, 227, 220, 224, 242, 255, 255, 255, 255},
        {255, 255, 232, 146, 42, 13, 0, 13, 38, 134, 226, 255, 255},
        {255, 221, 53, 0, 0, 0, 0, 0, 0, 0, 38, 213, 255},
        {247, 85, 0, 13, 150, 224, 242, 229, 161, 22, 0, 71, 244},
        {200, 0, 0, 158, 254, 255, 255, 255, 255, 170, 0, 0, 190},
        {151, 0, 22, 239, 255, 255, 255, 255, 255, 244, 34, 0, 141},
        {98, 0, 104, 252, 255, 255, 255, 255, 255, 254, 118, 0, 88},
        {53, 0, 127, 255, 255, 255, 255, 255, 255, 255, 142, 0, 38},
        {42, 0, 129, 255, 255, 255, 255, 255, 255, 255, 144, 0, 22},
        {83, 0, 110, 253, 255, 255, 255, 255, 255, 254, 125, 0, 73},
        {137, 0, 46, 245, 255, 255, 255, 255, 255, 249, 59, 0, 127},
        {188, 0, 0, 183, 255, 255, 255, 255, 255, 194, 0, 0, 178},
        {237, 50, 0, 38, 196, 245, 255, 247, 205, 53, 0, 34, 232},
        {255, 194, 13, 0, 0, 56, 73, 61, 13, 0, 0, 183, 255},
        {255, 254, 203, 83, 0, 0, 0, 0, 0, 73, 193, 254, 255},
        {255, 255, 255, 249, 222, 196, 187, 194, 218, 246, 255, 255, 255}
    };
    
    
}
    
#endif //GRIDWAVEIMAGES_H
