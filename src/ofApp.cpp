#include "ofApp.h"

ps_decoder_t *ps;
cmd_ln_t *config;
std::string sentence;
std::vector<new_utterance> result;
std::vector<std::string> files;
char const *hyp = "Press 'n' for a new file.";

std::vector<std::string> filename_split;
std::string directoryString = "/Users/benjgorman/Desktop/CUNY/old_cleaned/converted_raw/";
//std::string directoryString = "/Users/benjgorman/Desktop/CUNY/old_cleaned/cleaned/converted_raw/";

//--------------------------------------------------------------
void ofApp::setup()
{
    //some path, may be absolute or relative to bin/data
    ofDirectory dir(directoryString);
    //only show png files
    dir.allowExt("raw");
    //populate the directory object
    dir.listDir();
    
    //push the file names into our vector
    for(int i = 0; i < dir.numFiles(); i++)
    {
        files.push_back(dir.getName(i));
       
    }
    
    // in setup:
    myfont.loadFont("helvetica.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // in draw:
    myfont.drawString(sentence +"\n", 100,100);
}


void ofApp::process_result()
{
    int frame_rate = cmd_ln_int32_r(config, "-frate");
    ps_seg_t *iter = ps_seg_iter(ps, NULL);
    printf("\n\n");
    
    while (iter != NULL)
    {
        int32 sf, ef, pprob;
        float conf;
        
        ps_seg_frames(iter, &sf, &ef);
        pprob = ps_seg_prob(iter, NULL, NULL, NULL);
        conf = logmath_exp(ps_get_logmath(ps), pprob);
        
        //here is where we process the word
        
        new_utterance new_utt;
        new_utt.conf = conf;
        new_utt.sf = sf;
        new_utt.st = (float)sf / frame_rate;
        new_utt.ef = ef;
        new_utt.et = (float) ef / frame_rate;
        new_utt.utt = ps_seg_word(iter);
        
        printf("Recognised: %s %.3f %.3f %f\n", ps_seg_word(iter), new_utt.st,
               new_utt.et, new_utt.conf);
    
        std::string word = ps_seg_word(iter);
        
        result.push_back(new_utt);
        
        iter = ps_seg_next(iter);
        
    }
    
    printf("\n\n");
    
    engineExit();

}


bool ofApp::engineInit()
{
   config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", MODELDIR "/en-us/en-us", "-lm", MODELDIR "/en-us/en-us.lm.dmp", "-dict", MODELDIR "/en-us/cmudict-en-us.dict", NULL);
    
    
    //this is the setup you can use for microphone
    //config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", MODELDIR "/en-us/en-us", "-allphone", MODELDIR "/en-us/en-us-phone.lm.dmp", "-backtrace", "yes", "-beam", "1e-20", "-pbeam", "1e-20", "-lw", "2.0", NULL);
    
    if (config == NULL)
    {
        return false;
    }
    ps = ps_init(config);
    
    if (ps == NULL)
    {
        return false;
    }
}

int ofApp::engineExit()
{
    ps_free(ps);
    cmd_ln_free_r(config);
    return 0;
}

int ofApp::engineOpen(string filename)
{
    FILE *fh;
    char const *uttid;
    int16 buf[512];
    int rv; int32 score;

    fh = fopen((directoryString + filename).c_str(), "rb");
    if (fh == NULL)
    {
        return -1;
    }
    rv = ps_start_utt(ps);
    if (rv < 0) return 1;
    while (!feof(fh))
    {
        size_t nsamp; nsamp = fread(buf, 2, 512, fh);
        rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
    }
    
    engineClose();
}

int ofApp::engineClose()
{
    char const *uttid;
    int rv;
    int32 score;
    
    rv = ps_end_utt(ps);
    if (rv < 0)
    {
        return 1;
    }
    hyp = ps_get_hyp(ps, &score);
    if (hyp == NULL)
    {
        return 1;
    }
    
    printf("NEWLINE_________\n\n\nRecognized: %s\n", hyp);
    
    sentence = hyp;
    
    process_result();

    
}

int ofApp::engineSentAudio(short *audioBuf, int audioSize)
{
    
}

bool ofApp::isEngineOpened()
{
    
}

char * ofApp::engineGetText()
{

}

int j =0;

//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
{
    if (key == 'n')
    {
        
        if (j<files.size())
        {
            bEngineInitialed = engineInit();
            

            engineOpen(files[j]);
            
        }
        j++;
        
    }
    
    if( key == 's' )
    {
        //soundStream.start();
        //ps_start_utt(ps);
    }
    
    if( key == 'e' )
    {
        //soundStream.stop();
        //engineClose();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}


