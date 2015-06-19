#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "MurmurContourTrackingApp.h"


//========================================================================
int main( ){

    ofAppGlutWindow window;
    ofSetupOpenGL(&window,1280, 1024, OF_WINDOW);
    ofRunApp( new MurmurContourTrackingApp());

}
