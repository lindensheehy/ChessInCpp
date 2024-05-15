#ifndef drawfuncs
#define drawfuncs

#include <SDL2/SDL.h>

#include "png.h"

class Drawer {
    public:
        
        // Instance variables
        Uint32* buffer;
        unsigned int bufferHeight;
        unsigned int bufferWidth;

        // Constructor
        Drawer(Uint32* buffer, unsigned int bufferWidthInput, unsigned int bufferHeightInput) {
            this->buffer = buffer;
            this->bufferWidth = bufferWidthInput;
            this->bufferHeight = bufferHeightInput;
        }

        // Instance functions
        void writePixel(Uint32 pixel, int x, int y) {

            if (this->buffer == NULL) {
                return;
            }

            buffer[ (y * this->bufferHeight) + x ] = pixel;

            return;
        }

        void drawLine(Uint32 pixel, int startx, int starty, int endx, int endy) {

            if (this->buffer == NULL) {
                return;
            }
            
        }

        void drawRect(Uint32 pixel, int startx, int starty, int endx, int endy) {

            if (this->buffer == NULL) {
                return;
            }

            int incerementI = 1;
            int incerementJ = 1;

            if ( startx > endx ) incerementI = -1;
            if ( starty > endy ) incerementJ = -1;

            for (int i = startx; i != endx; i += incerementI) {
                for (int j = starty; j != endy; j += incerementJ) {
                    this->writePixel(pixel, i, j);
                }
            }

            return;
        }

        void drawpng(PNG* file, int x, int y) {

            if (x + file->width > bufferWidth) return;
            if (y + file->height > bufferHeight) return;
            
            if (this->buffer == NULL) {
                return;
            }

            for (int i = x; i < (x + file->width); i++) {
                for (int j = y; j < (y + file->height); j++) {

                    Color* pixel = file->getPixel(i - x, j - y);

                    if (pixel->opacityValue != 0x00) {
                        this->writePixel(pixel->rawValue, i, j);
                    }

                }
            }
            
            return;
        }

    private:

};

#endif