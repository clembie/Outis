	#include "ofApp.h"
    #include "radialPatch.h"

    #include <iostream>
    #include <fstream>
    #include <stdio.h>
    #include <string.h>

using namespace std;


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("Outis");
    ofSetWindowShape(1280, 720);
    
    camWidth = 1280;
    camHeight = 720;
    
#ifdef _USE_LIVE_VIDEO
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth,camHeight);
#else
    cout << "error: no camera attached!";
    vidPlayer.loadMovie("firetest2.mov");
    vidPlayer.play();
    
    // VIDEO
    //loopState = OF_QT_LOOP_NORMAL;
    loopStart = 0.0;
    loopStop = 1.0;
    playSpeed = 1.0;
#endif
    
    colorImg.allocate(camWidth,camHeight);
    grayImage.allocate(camWidth,camHeight);
    grayBg.allocate(camWidth,camHeight);
    grayDiff.allocate(camWidth,camHeight);
    
    colorPixels = new unsigned char [camWidth*camHeight*3];
    
    bLearnBakground = true;
    setPatch = false;
    threshold = 40;
    pixelSize = 1;
    offsetX = 0;
    offsetY = 0;
    numBlobs = 20;
    blobSizeMin = 10;
    blurAmt = 3;
    posterizeLevel = 10;
    frameNumber = 0;
    focalX = camWidth/2;
    focalY = camHeight/2;
    focalRadius = 100;
    offsetValue = 0;
    fractalAmt = 0;
    
    send = false;
    blur = false;
    GUI = true;
    HUD = true;
    BW = false;
    BWFull = false;
    drawPatch = false;
    info = false;
    posterize = true;
    invert = false;
    offset = true;
    fractal = true;
    contrastStretching = true;
    black = false;
    
    // open an outgoing connection to HOST:PORT - HOST2 can be a second receiver...
    sender.setup( HOST, OUTPORT );
    //sender2.setup( HOST2, OUTPORT );
    receiver.setup( INPORT );
    
    // setup radpatch
    radpatch = radialPatch(float(focalX), float(focalY), 12, 5, float(focalRadius));
    
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100,100,100);
    
    bool bNewFrame = false;
    
    // UPDATE MESSAGES
    while( receiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        // check for focal position
        if( std::strcmp( m.getAddress().c_str(), "/focal/position" ) == 0 )
        {
            focalX = m.getArgAsInt32( 0 );
            focalY = m.getArgAsInt32( 1 );
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/focal/radius" ) == 0 )
        {
            focalRadius = m.getArgAsInt32( 0 );
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/patch/set" ) == 0 )
        {
            pie = m.getArgAsInt32( 0 );
            slice = m.getArgAsInt32( 1 );
            col = m.getArgAsInt32( 2 );
            
            radpatch.set(pie, slice, col);
        }
        
        // game controller
        
        else if ( strcmp( m.getAddress().c_str(), "/control/threshold" ) == 0 )
        {
            // int
            threshold = m.getArgAsInt32(0);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/blur" ) == 0 )
        {
            // int (toggle blur)
            toggle(blur);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/send" ) == 0 )
        {
            // int (toggle send)
            toggle(send);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/bluramt" ) == 0 )
        {
            // int
            modBlurAmt(m.getArgAsInt32(0));
            //if(blurAmt < 0)
            //	blurAmt = 0;
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/posterize" ) == 0 )
        {
            // int
            toggle(posterize);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/postlevel" ) == 0 )
        {
            // int
            posterizeLevel = m.getArgAsInt32(0);
            if(posterizeLevel < 1)
                posterizeLevel = 1;
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/invert" ) == 0 )
        {
            // int (toggle invert)
            toggle(invert);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/contrast" ) == 0 )
        {
            // int (toggle contrastStretching)
            toggle(contrastStretching);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/blobsizemin" ) == 0 )
        {
            // int
            modBlobSizeMin(m.getArgAsInt32(0));
            //if(blobSizeMin < 0)
            //	blobSizeMin = 0;
        }
        
        // video only
        else if ( strcmp( m.getAddress().c_str(), "/control/setin" ) == 0 )
        {
            // int (set in to now)
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = vidPlayer.getPosition();
#endif
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/setout" ) == 0 )
        {
            // int (set out to now)
#ifdef _USE_LIVE_VIDEO
#else
            loopStop = vidPlayer.getPosition();
#endif
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/droploop" ) == 0 )
        {
            loopStart = 0.0;
            loopStop = 1.0;
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/speed" ) == 0 )
        {
            // float
            playSpeed = m.getArgAsFloat(0);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/reverse" ) == 0 )
        {
            // int (reverse speed)
            playSpeed = 0 - playSpeed;
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/stop" ) == 0 )
        {
            // int (set speed to 0)
            playSpeed = 0;
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/normspeed" ) == 0 )
        {
            // int (set speed to 1)
            playSpeed = 1;
        }
        
        // reset
        
        else if ( strcmp( m.getAddress().c_str(), "/control/reset" ) == 0 )
        {
            // int (reset to defaults) ??
        }
        
        // glitch control
        
        else if ( strcmp( m.getAddress().c_str(), "/control/offset" ) == 0 )
        {
            // int (toggle offset)
            toggle(offset);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/fractal" ) == 0 )
        {
            // int (toggle fractal)
            toggle(fractal);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/offsetvalue" ) == 0 )
        {
            // int (offsetValue)
            offsetValue = m.getArgAsInt32(0);
        }
        
        else if ( strcmp( m.getAddress().c_str(), "/control/fractalamt" ) == 0 )
        {
            // int (fractalAmt)
            fractalAmt = m.getArgAsInt32(0);
        }
        
        // end game controller
        
        else
        {
            cout << "unrecognized OSC msg: ";
            cout << m.getAddress();
            cout << "\n";
        }
    }
    
    
#ifdef _USE_LIVE_VIDEO
    vidGrabber.update();
    //vidGrabber.grabFrame();
    bNewFrame = vidGrabber.isFrameNew();
#else
    vidPlayer.idleMovie();
    bNewFrame = vidPlayer.isFrameNew();
    
    // VIDEO STATE
    vidPlayer.setSpeed(playSpeed);
#endif
    
    // UPDATE PATCH
    radpatch.update(focalX, focalY, focalRadius);
    
    // UPDATE FRAME STUFF
    //if (bNewFrame){
    frameNumber++;
    
#ifdef _USE_LIVE_VIDEO
    colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
#else
    colorImg.setFromPixels(vidPlayer.getPixels(), camWidth,camHeight);
#endif
    
    colorPixels= colorImg.getPixels();
    
    if( posterize ) {
        // pixel-by-pixel intervention
        for( int x = 0; x < camWidth; x++ ) {
            for( int y = 0; y < camHeight; y++ ) {
                colorPixels[(y*camWidth+x)*3 + 0] = (colorPixels[(y*camWidth+x)*3 + 0] / posterizeLevel) * posterizeLevel;	// r
                colorPixels[(y*camWidth+x)*3 + 1] = (colorPixels[(y*camWidth+x)*3 + 1] / posterizeLevel) * posterizeLevel;	// g
                colorPixels[(y*camWidth+x)*3 + 2] = (colorPixels[(y*camWidth+x)*3 + 2] / posterizeLevel) * posterizeLevel;	// b
            }
        }
        colorImg.setFromPixels(colorPixels, camWidth, camHeight);
    }
    
    if( invert ) {
        // pixel-by-pixel intervention
        for( int x = 0; x < camWidth; x++ ) {
            for( int y = 0; y < camHeight; y++ ) {
                colorPixels[(y*camWidth+x)*3 + 0] = 255 - colorPixels[(y*camWidth+x)*3 + 0];	// r
                colorPixels[(y*camWidth+x)*3 + 1] = 255 - colorPixels[(y*camWidth+x)*3 + 1];	// g
                colorPixels[(y*camWidth+x)*3 + 2] = 255 - colorPixels[(y*camWidth+x)*3 + 2];	// b
            }
        }
        colorImg.setFromPixels(colorPixels, camWidth, camHeight);
    }
    /*  // interesting, however, incorrect...
     if( offset )
     {
     // pixel-by-pixel intervention
     for( int x = 0; x < camWidth; x++ ) {
     for( int y = 0; y < camHeight; y++ ) {
					colorPixels[(y*camWidth+x)*3 + 0] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth*camHeight))*3 + 0];	// r
					colorPixels[(y*camWidth+x)*3 + 1] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth*camHeight))*3 + 1];	// g
					colorPixels[(y*camWidth+x)*3 + 2] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth*camHeight))*3 + 2];	// b
     }
     }
     colorImg.setFromPixels(colorPixels, camWidth, camHeight);
     }
     */
    
    // skewz
    if( offset )
    {
        // glitchy skewz
        for( int x = 0; x < camWidth; x++ ) {
            for( int y = 0; y < camHeight; y++ ) {
                colorPixels[(y*camWidth+x)*3 + 0] = colorPixels[((y*camWidth+(x+offsetValue+y*offsetValue))%(camWidth*camHeight))*3 + 0];	// r
                colorPixels[(y*camWidth+x)*3 + 1] = colorPixels[((y*camWidth+(x+offsetValue+y*offsetValue))%(camWidth*camHeight))*3 + 1];	// g
                colorPixels[(y*camWidth+x)*3 + 2] = colorPixels[((y*camWidth+(x+offsetValue+y*offsetValue))%(camWidth*camHeight))*3 + 2];	// b
            }
        }
        colorImg.setFromPixels(colorPixels, camWidth, camHeight);
    }
    
    // fractalz!!
    if( fractal )
    {
        // glitttchcchch!!!11!1  FRAGD
        for( int i = 0; i < (camWidth*camHeight*3); i++ ) {
            colorPixels[i] = colorPixels[(i+fractalAmt*i)%(camWidth*camHeight*3)];
        }
        colorImg.setFromPixels(colorPixels, camWidth, camHeight);
    }
    /*
     
     if( offset )
     {
     // glitttchcchch!!!11!1
     for( int x = 0; x < camWidth; x++ ) {
     for( int y = 0; y < camHeight; y++ ) {
					colorPixels[(y*camWidth+x)*3 + 0] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth))*3 + 0];	// r
					colorPixels[(y*camWidth+x)*3 + 1] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth))*3 + 1];	// g
					colorPixels[(y*camWidth+x)*3 + 2] = colorPixels[((y*camWidth+(x+offsetValue))%(camWidth))*3 + 2];	// b
     }
     }
     colorImg.setFromPixels(colorPixels, camWidth, camHeight);
     }
     
     
     if( offset )
     {
     // pixel-by-pixel intervention
     for( int x = 0; x < camWidth; x++ ) {
     for( int y = 0; y < camHeight; y++ ) {
					colorPixels[(y*camWidth+x)*3 + 0] = colorPixels[((y*camWidth+(x+offsetValue+y))%(camWidth*camHeight))*3 + 0];	// r
					colorPixels[(y*camWidth+x)*3 + 1] = colorPixels[((y*camWidth+(x+offsetValue+y))%(camWidth*camHeight))*3 + 1];	// g
					colorPixels[(y*camWidth+x)*3 + 2] = colorPixels[((y*camWidth+(x+offsetValue+y))%(camWidth*camHeight))*3 + 2];	// b
     }
     }
     colorImg.setFromPixels(colorPixels, camWidth, camHeight);
     }
     */
    
    if( black )
    {
        // glitttchcchch!!!11!1  FRAGD
        for( int i = 0; i < (camWidth*camHeight*3); i++ ) {
            colorPixels[i] = 0;
        }
        colorImg.setFromPixels(colorPixels, camWidth, camHeight);
    }
    
    // blur
    if( blur ) {
        colorImg.blurGaussian( blurAmt );
    }
    
    // create grayscale image to do blob detection on
    grayImage = colorImg;
    
    // maybe contrast stretch
    if( contrastStretching ) {
        grayImage.contrastStretch();
    }
    
    // threshold the gray image
    grayImage.threshold(threshold);
    
    contourFinder.findContours(grayImage, blobSizeMin, (340*camHeight)/3, numBlobs, true);	// find holes
    
    //------------------------------------------------------
    // SEND STUFF
    
    sendFrameUpdate();
    
    if( send ) {
        sendOsc( contourFinder );
    }
    
#ifdef _USE_LIVE_VIDEO
#else
    // VIDEO LOOP STATE
    if( vidPlayer.getPosition() > loopStop )
        vidPlayer.setPosition( loopStart );
    else if( vidPlayer.getPosition() < loopStart )	// e.g. if we're going backwards
        vidPlayer.setPosition( loopStop );
#endif
    
    //}
}


//--------------------------------------------------------------
void ofApp::draw(){
    //------------------------------------------------------
    ofSetHexColor(0xffffff);
    
    colorImg.draw(0,0);
    //grayDiff.draw(camWidth,0);
    if(BW)
    {
        if(BWFull)
            grayImage.draw(0,0, camWidth, camHeight);
        else
            grayImage.draw(camWidth-180,0, 180, 120);
    }
    
    //------------------------------------------------------
    // then draw the contours and GUI:
    
    if( GUI ){
        // BLOBS
        char blobInfo[128];
        for (int i = 0; i < contourFinder.nBlobs; i++){
            ofFill();
            contourFinder.blobs[i].draw(0,0);
            ofSetColor(255,130,0);
            ofFill();		// draw "filled shapes"
            ofCircle(contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, 2);
            if( info ) {
                sprintf(blobInfo, "cent:%.2fx%.2f\narea:%.2f\nlen:%.2f\nrect:%.2fx%.2f", contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y, contourFinder.blobs[i].area, contourFinder.blobs[i].length, contourFinder.blobs[i].boundingRect.width, contourFinder.blobs[i].boundingRect.height);
                ofDrawBitmapString(blobInfo, contourFinder.blobs[i].centroid.x, contourFinder.blobs[i].centroid.y + 10);
            }
        }
        
        
        // toggle "send" display
        if(send) {
            ofSetHexColor(0x00ff00);
        }
        else {
            ofSetHexColor(0xff0000);
        }
        
        ofFill();
        ofCircle(camWidth-10, 10, 5);
        
        if( HUD ) {
            //------------------------------------------------------
            // write text to the screen:
            ofSetHexColor(0x00ff00);
            ofFill();
            //char reportStr[1024];
            //sprintf(reportStr, "thr %i\nmxb %i\nmns %i\nnum blb found %i\nblr %i amt %i\npst %i\ncst %i", threshold, numBlobs, blobSizeMin, contourFinder.nBlobs, blur, blurAmt, posterizeLevel, contrastStretching);
            //cout << reportStr;
            //ofDrawBitmapString(reportStr, 20, 20);
            
            int texty = 20;
            int textx = 10;
            int rx = textx*4;
            int yoffset = 15;
            int recty = texty-8;
            
            ofDrawBitmapString("thr", textx, texty);
            ofRect(rx, recty, threshold, 10);
            
            ofDrawBitmapString("mxb", textx, texty+(yoffset*1));
            ofRect(rx, recty+(yoffset*1), numBlobs, 10);
            
            ofDrawBitmapString("mns", textx, texty+(yoffset*2));
            ofRect(rx, recty+(yoffset*2), blobSizeMin, 10);
            
            ofDrawBitmapString("blb", textx, texty+(yoffset*3));
            ofRect(rx, recty+(yoffset*3), contourFinder.nBlobs, 10);
            
            if( !blur )
                ofSetHexColor(0xcccccc);
            else
                ofSetHexColor(0x00ff00);
            
            ofDrawBitmapString("blr", textx, texty+(yoffset*4));
            ofRect(rx, recty+(yoffset*4), blurAmt, 10);
            
            if( !posterize )
                ofSetHexColor(0xcccccc);
            else
                ofSetHexColor(0x00ff00);
            
            ofDrawBitmapString("pst", textx, texty+(yoffset*5));
            ofRect(rx, recty+(yoffset*5), posterizeLevel, 10);
            
            if( !contrastStretching )
                ofSetHexColor(0xcccccc);
            else
                ofSetHexColor(0x00ff00);
            
            ofDrawBitmapString("cst", textx, texty+(yoffset*6));
        }
    }
    
    
    if(drawPatch)
    {
        // FOCAL AREA
        ofSetHexColor(0xff0000);
        ofNoFill();
        ofCircle(focalX, focalY, focalRadius);
        ofFill();
        ofCircle(focalX, focalY, 2);
        
        // draw radpatch
        radpatch.draw(50);
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch (key){
        case ' ':
            bLearnBakground = true;
            break;
        case '=':
            modThreshold(5);
            break;
        case '-':
            modThreshold(-5);
            break;
            
        case '[':
            modNumBlobs(-1);
            break;
        case ']':
            modNumBlobs(1);
            break;
            
        case '{':
            modBlurAmt(-2);
            break;
        case '}':
            modBlurAmt(2);
            break;
            
        case ';':
            modBlobSizeMin(-1);
            break;
        case '\'':
            modBlobSizeMin(1);
            break;
            
        case '<':
            modPosterizeLevel(-1);
            break;
        case '>':
            modPosterizeLevel(1);
            break;
            
        case 's':
            toggle(send);
            break;
        case 't':
            ofToggleFullscreen();
            break;
        case '!':
            //vidGrabber.videoSettings();
            break;
        case 'a':
            sendTestMsg();	// send a test OSC message.
            break;
        case 'b':
            toggle(blur);
            break;
        case 'p':
            toggle(posterize);
            break;
        case 'i':
            toggle(invert);
            break;
        case 'o':
            toggle(offset);
            break;
        case '9':
            modOffsetValue(-1);
            break;
        case '0':
            modOffsetValue(1);
            break;
        case 'g':
            toggle(GUI);
            toggle(black);
            break;
        case 'h':
            toggle(HUD);
            break;
        case 'w':
            toggle(BW);
            break;
        case 'e':
            toggle(BWFull);
            break;
        case 'y':
            toggle(info);
            break;
        case 'j':
            toggle(drawPatch);
            break;
        case 'c':
            toggle(contrastStretching);
            break;
            // VIDEO
        case ',':
            playSpeed -= 0.1;
            break;
        case '.':
            playSpeed += 0.1;
            break;
        case 'l':
#ifdef _USE_LIVE_VIDEO
#else
            /*
             if( loopState == OF_QT_LOOP_NORMAL )
             loopState == OF_QT_LOOP_PALINDROME;
             else if( loopState == OF_QT_LOOP_PALINDROME )
             loopState == OF_QT_LOOP_NORMAL;
             */
#endif
            break;
        case 'z':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = vidPlayer.getPosition();
            //cout << "in: ";
            //cout << loopStart;
            //cout << "\n";
#endif
            break;
        case 'x':
#ifdef _USE_LIVE_VIDEO
#else
            loopStop = vidPlayer.getPosition();
            //cout << "out: ";
            //cout << loopStop;
            //cout << "\n";
#endif
            break;
        case 'd':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = 0.0;
            loopStop = 1.0;
#endif
            break;
        case 'n':
            playSpeed = 1.0;
            break;
            
        case '*':
            cout << "in: ";
            cout << loopStart;
            cout << " out: ";
            cout << loopStop;
            cout << " thr: ";
            cout << threshold;
            cout << " mxb: ";
            cout << numBlobs;
            cout << " mns: ";
            cout << blobSizeMin;
            
            if(posterize)
            {
                cout << " pst: ";
                cout << posterizeLevel;
            }
            
            if(contrastStretching)
                cout << " cst: true ";
            
            if(invert)
                cout << " inv: true ";
            
            if(blur)
            {
                cout << " blr: ";
                cout << blurAmt;
            }
            
            cout << " spd: ";
            cout << playSpeed;
            
            cout << "\n";
            
            break;
            
            // PRESETS
        case '1':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = 0.398825;
            loopStop = 0.425788;
            threshold = 49;
            numBlobs = 20;
            blobSizeMin = 6;
            posterize = true;
            posterizeLevel = 24;
            contrastStretching = true;
            blur = false;
            blurAmt = 0;
            invert = true;
            playSpeed = 0.2;
            vidPlayer.setPosition(loopStart);
#endif
            break;
            
        case '2':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = 0.586012;
            loopStop = 0.602337;
            threshold = 119;
            numBlobs = 20;
            blobSizeMin = 1;
            posterize = false;
            contrastStretching = true;
            blur = true;
            blurAmt = 19;
            invert = false;
            playSpeed = 0.1;
            vidPlayer.setPosition(loopStart);
#endif
            break;
            
        case '3':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = 0.644912;
            loopStop = 0.672925;
            threshold = 67;
            numBlobs = 20;
            blobSizeMin = 1;
            posterize = true;
            posterizeLevel = 21;
            contrastStretching = true;
            blur = true;
            blurAmt = 19;
            invert = true;
            playSpeed = 0.2;
            vidPlayer.setPosition(loopStart);
#endif
            break;	
            
        case '4':
#ifdef _USE_LIVE_VIDEO
#else
            loopStart = 0.72195;
            loopStop = 0.767362;
            threshold = 0;
            numBlobs = 20;
            blobSizeMin = 1;
            posterize = true;
            posterizeLevel = 80;
            contrastStretching = false;
            blur = true;
            blurAmt = 19;
            invert = false;
            playSpeed = 0.3;
            vidPlayer.setPosition(loopStart);
#endif
            break;
            
        default:
            cout << "unrecognized keyPress event ";
            cout << key;
            cout << "\n";
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}	

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(){
    
}

//--------------------------------------------------------------
int ofApp::getX(int i) {
    return (i/3)%camWidth;		// for RGB
    //return (i%camWidth);			// greyscale
}

int ofApp::getY(int i) {
    return i/3/camWidth;		// for RGB
    //return (i/camWidth);			// greyscale
}


//--------------------------------------------------------------
void ofApp::sendOsc( ofxCvContourFinder & contour ) {
    //cout << "sending data...\n";
    
    // send blobs
    for (int i = 0; i < contour.nBlobs; i++){
        ofxOscMessage blobInfoMsg;
        blobInfoMsg.setAddress("/blob");   // iiffffffffiiiii
        blobInfoMsg.addIntArg(frameNumber);
        
        // curent number of blobs in frame
        blobInfoMsg.addIntArg(contour.nBlobs);
        
        //cout << contour.nBlobs;
        
        blobInfoMsg.addFloatArg(contour.blobs[i].area);
        blobInfoMsg.addFloatArg(contour.blobs[i].length);
        blobInfoMsg.addFloatArg(contour.blobs[i].boundingRect.x);
        blobInfoMsg.addFloatArg(contour.blobs[i].boundingRect.y);
        blobInfoMsg.addFloatArg(contour.blobs[i].boundingRect.width);
        blobInfoMsg.addFloatArg(contour.blobs[i].boundingRect.height);
        blobInfoMsg.addFloatArg(contour.blobs[i].centroid.x);
        blobInfoMsg.addFloatArg(contour.blobs[i].centroid.y);
        blobInfoMsg.addIntArg(colorPixels[int((contour.blobs[i].centroid.y * camWidth + contour.blobs[i].centroid.x) * 3 + 0)]);	// red at cent
        blobInfoMsg.addIntArg(colorPixels[int((contour.blobs[i].centroid.y * camWidth + contour.blobs[i].centroid.x) * 3 + 1)]);	// green at cent
        blobInfoMsg.addIntArg(colorPixels[int((contour.blobs[i].centroid.y * camWidth + contour.blobs[i].centroid.x) * 3 + 2)]);	// blue at cent
        if(contour.blobs[i].hole) {
            blobInfoMsg.addIntArg(1);
        }
        else {
            blobInfoMsg.addIntArg(0);
        }
        blobInfoMsg.addIntArg(contour.blobs[i].nPts);
        sender.sendMessage( blobInfoMsg );
        //sender2.sendMessage( blobInfoMsg );
    }
}

void ofApp::sendFrameUpdate() {
    ofxOscMessage frInfoMsg;
    frInfoMsg.setAddress( "/frame" );	// i
    frInfoMsg.addIntArg( frameNumber );
    sender.sendMessage( frInfoMsg );
}


//--------------------------------------------------------------
void ofApp::sendTestMsg() {
    ofxOscMessage testMsg;
    testMsg.setAddress( "/test" );	// f
    testMsg.addFloatArg( 500.0f );
    sender.sendMessage( testMsg );
}

void ofApp::toggle( bool & b ) {
    b = !b;
}

void ofApp::modPosterizeLevel( int n ) {
    posterizeLevel = posterizeLevel + n;
    if(posterizeLevel < 1) 
        posterizeLevel  = 1;
}

void ofApp::modBlobSizeMin( int n ) {
    blobSizeMin = blobSizeMin + n;
    if(blobSizeMin < 1) 
        blobSizeMin  = 1;
}

void ofApp::modBlurAmt( int n ) {
    if( n % 2 != 0 )	// adder is not even
        n++;			// make even - blurAmt must be odd
    blurAmt = blurAmt + n;
    if( blurAmt < 1 ) 
        blurAmt = 1;
    if( blurAmt > 81 )
        blurAmt = 81;
}

void ofApp::modNumBlobs( int n ) {
    numBlobs = numBlobs + n;
    if( numBlobs < 1 ) 
        numBlobs = 1;
}

void ofApp::modThreshold( int n ) {
    threshold = threshold + n;
    if( threshold < 0 ) 
        threshold = 0;
    if( threshold > 255 ) 
        threshold = 255;
}

void ofApp::modOffsetValue( int n ) {
    offsetValue = offsetValue + n;
    /*if( offsetValue < 0 ) 
     offsetValue = 0;
     if( offsetValue > camWidth ) 
     offsetValue = camWidth;*/
}