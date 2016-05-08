
/* 
 * File:   driver.cpp
 * Author: amy
 *
 * Created on 06 May 2016, 2:51 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include "Audio.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    string temp;
    int sampleRate;
    int bitSize;
    int channel;
    string outputFile = "%", soundFile1, soundFile2;
    int r1, r2, s1, s2;
    float fr1, fr2;
    const int mono=1;
    const int stereo =2;
    
    for (int i=0; i<argc; i++){
        temp = string(argv[i]);
        if (temp == "-r"){
            temp = string(argv[i+1]);
            istringstream iss(temp);
            iss >> sampleRate;
        }
        else if (temp == "-b"){
            temp = string(argv[i+1]);
            if (temp != "16" && temp != "8"){
                cerr << "Only 8bit and 16bit are supported." << endl;
                return 0;
            }
            else{
                istringstream iss(temp);
                iss >> bitSize;
            }
        }
        else if (temp == "-c"){
            temp = string(argv[i+1]);
            if (temp != "1" && temp != "2"){
                cerr << "Only mono and stereo channels are supported." << endl;
                return 0;
            }
            else{
                istringstream iss(temp);
                iss >> channel;
            }
        }
        else if (temp == "-o"){
            outputFile = string(argv[i+1]);
            soundFile1 = string(argv[i+2]);
            soundFile2 = string(argv[i+3]);
            BRDAMY004::Audio<int8_t, mono> audio(sampleRate, bitSize, channel);
            audio.loadToBuffer(soundFile1);
            //audio.writeToFile(outputFile);
            BRDAMY004::Audio<int8_t, mono> audio2(sampleRate, bitSize, channel);
            audio2.loadToBuffer(soundFile2);
            //audio.concatenate(audio2);
            //audio.volume(0.75f,0.0f);
            //audio.add(audio2);
            //audio.cut(300000, 600000);
            //audio.reverse();
            audio.writeToFile(outputFile);
        }
        else if (temp == "-add"){
            soundFile1 = string(argv[i+1]);
            soundFile2 = string(argv[i+2]);
            break;
        }
        else if (temp == "-cut"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> r1 >> r2;
            soundFile1 = string(argv[i+3]);
            break;
        }
        else if (temp == "-radd"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]) + " " + string(argv[i+3]) + " " + string(argv[i+4]);
            istringstream iss(temp);
            iss >> r1 >> r2 >> s1 >> s2;
            soundFile1 = string(argv[i+5]);
            soundFile2 = string(argv[i+6]);
            break;
        }
        else if (temp == "-cat"){
            soundFile1 = string(argv[i+1]);
            soundFile2 = string(argv[i+2]);
            break;
        }
        else if (temp == "-v"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> fr1 >> fr2;
            soundFile1 = string(argv[i+3]);
            break;
        }
        else if (temp == "-rev"){
            soundFile1 = string(argv[i+1]);
            break;
        }
        else if (temp == "-rms"){
            soundFile1 = string(argv[i+1]);
            break;
        }
        else if (temp == "-norm"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> fr1 >> fr2;
            soundFile1 = string(argv[i+3]);
            break;
        }
    }
    
    return 0;
}

