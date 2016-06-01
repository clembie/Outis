#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "radialPatch.h"
#include "ofxOpenCv.h"
#include "ofxOsc.h"

#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOSC

// #include "ofAddons.h"

#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
// otherwise, we'll use a movie file

#define HOST "localhost"
#define HOST2 "192.168.1.102"
#define OUTPORT 57120
#define INPORT	12346


class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased();
    
    void sendOsc( ofxCvContourFinder & contour );
    void sendTestMsg();
    void sendFrameUpdate();
    
    void toggle( bool & b );
    void modPosterizeLevel( int n );
    void modBlobSizeMin( int n );
    void modBlurAmt( int n );
    void modNumBlobs( int n );
    void modThreshold( int n );
    void modOffsetValue( int n );
    
    
    
    int getX(int i);
    int getY(int i);
    
    
#ifdef _USE_LIVE_VIDEO
		  ofVideoGrabber 		vidGrabber;
#else
		  ofVideoPlayer 		vidPlayer;
#endif
    
    ofxCvColorImage			colorImg;
    
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    ofxCvGrayscaleImage 	grayDiffDisp;
    
    
    ofxCvContourFinder 	contourFinder;
    
    int					camWidth;
    int					camHeight;
    int 				threshold;
    int					pixelSize;
    int					offsetX;
    int					offsetY;
    int					numBlobs;
    int					blobSizeMin;
    int					blurAmt;
    int					posterizeLevel;
    int					frameNumber;
    int					focalX;
    int					focalY;
    int					focalRadius;
    int					pie;
    int					slice;
    int					col;
    int					offsetValue;
    int					fractalAmt;
    
    // VIDEO
    int					loopState;
    float				loopStart;
    float				loopStop;
    float				playSpeed;
    
    bool				bLearnBakground;
    bool				setPatch;
    bool				send;
    bool				blur;
    bool				GUI;
    bool				info;
    bool				HUD;
    bool				BW;
    bool				BWFull;
    bool				drawPatch;
    bool				contrastStretching;
    bool				posterize;
    bool				invert;
    bool				offset;
    bool				fractal;
    bool				black;
    
    unsigned char *		colorPixels;
    
    radialPatch			radpatch;
    
    
private:
    
    ofxOscSender		sender;
    ofxOscSender		sender2;
    ofxOscReceiver		receiver;
    
};

#endif
