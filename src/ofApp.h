#pragma once

#include "ofMain.h"
#include "pocketsphinx.h"

#define MODELDIR "/Users/benjgorman/desktop"

class new_utterance
{
public:
    
    float conf;
    float sf;
    float st;
    float ef;
    float et;
    string utt="";
    
};

class ofApp : public ofBaseApp
{
    private:
    // The Decoder
    fe_t *fe;
    
    // States
    bool bEngineInitialed = false;
    bool bEngineOpened;
    
    // Utterance number
    int uttnum;

	public:
        void process_result();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        //saves between loads
        
    
        ps_decoder_t *ps;
        cmd_ln_t *config;
    

        // Operation of the speech Engine
        bool engineInit();
        int engineExit();
        int engineOpen(std::string filename);
        int engineClose();
        int engineSentAudio(short *audioBuf, int audioSize);
        bool isEngineOpened();
        char * engineGetText();
    
    ofTrueTypeFont myfont;
    
};
