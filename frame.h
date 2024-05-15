#ifndef frame
#define frame

#include <SDL2/SDL.h>

class FrameState {

    public:

        class MouseState {
            public:

                // Instance variables
                bool leftButtonIsDown;
                bool rightButtonIsDown;
                int posX;
                int posY;

                // Contructor
                MouseState() {
                    this->leftButtonIsDown = false;
                    this->rightButtonIsDown = false;
                    this->posX = 0;
                    this->posY = 0;
                }

                void leftButtonDown() {
                    this->leftButtonIsDown = true;
                }
                
                void leftButtonUp() {
                    this->leftButtonIsDown = false;
                }

                void rightButtonDown() {
                    this->rightButtonIsDown = true;
                }

                void rightButtonUp() {
                    this->rightButtonIsDown = false;
                }

                void setPos(int x, int y) {
                    this->posX = x;
                    this->posY = y;
                }

            private:
        };

        // Instance variables
        int frameCount;
        MouseState* mouse;
        FrameState* lastFrame;

        // Contructor
        FrameState(bool hasChild = true) {

            this->frameCount = 0;
            this->mouse = new MouseState();

            if (hasChild) this->lastFrame = new FrameState(false);
            else this->lastFrame = NULL;
        }

        // Destructor
        ~FrameState() {
            delete this->mouse;
            if (this->lastFrame != NULL) delete this->lastFrame;
        }

        // Instance functions
        void nextFrame() {
            delete this->lastFrame->mouse;
            this->lastFrame->mouse = this->mouse;
            this->mouse = new MouseState(*(this->mouse));
        }
    
        void incrementCounter() {
            this->frameCount++;
        }

        bool wasLeftJustPressed() {
            return (this->mouse->leftButtonIsDown && !this->lastFrame->mouse->leftButtonIsDown);
        }

        bool wasRightJustPressed() {
            return (this->mouse->rightButtonIsDown && !this->lastFrame->mouse->rightButtonIsDown);
        }

        bool wasLeftJustReleased() {
            return (!this->mouse->leftButtonIsDown && this->lastFrame->mouse->leftButtonIsDown);
        }

        bool wasRightJustReleased() {
            return (!this->mouse->rightButtonIsDown && this->lastFrame->mouse->rightButtonIsDown);
        }

    private:

};

#endif