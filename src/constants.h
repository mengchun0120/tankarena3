#ifndef INCLUDE_CONSTANTS
#define INCLUDE_CONSTANTS

namespace ta3 {

class Constants {
public:
    static const unsigned int NUM_FLOATS_PER_VERTEX = 2;
    static const unsigned int NUM_FLOATS_PER_TEXTURE_COORD = 2;
    static const unsigned int VERTEX_SIZE = sizeof(float) * NUM_FLOATS_PER_VERTEX;
    static const unsigned int TEXTURE_COORD_SIZE = sizeof(float) * NUM_FLOATS_PER_TEXTURE_COORD;
    static const unsigned int VERTEX_TEX_COORD_SIZE = VERTEX_SIZE + TEXTURE_COORD_SIZE;
};

} // end of namespace ta3


#endif
