
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
    string outputFile = "out", soundFile1, soundFile2;
    int r1, r2, s1, s2;
    float fr1, fr2;
    const int mono=1;
    const float stereo =2.0f;
    
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
        }
        else if (temp == "-add"){
            soundFile1 = string(argv[i+1]);
            soundFile2 = string(argv[i+2]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.add(audio1);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.add(audio1);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.add(audio1);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.add(audio1);
                    audio.writeToFile(outputFile);
                }
            }
           
            break;
        }
        else if (temp == "-cut"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> r1 >> r2;
            soundFile1 = string(argv[i+3]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.cut(r1, r2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.cut(r1, r2);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.cut(r1, r2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.cut(r1, r2);
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
        else if (temp == "-radd"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]) + " " + string(argv[i+3]) + " " + string(argv[i+4]);
            istringstream iss(temp);
            iss >> r1 >> r2 >> s1 >> s2;
            soundFile1 = string(argv[i+5]);
            soundFile2 = string(argv[i+6]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.rangedAdd(audio1, r1, r2, s1, s2);
                    audio.writeToFile(outputFile);
                    
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.rangedAdd(audio1, r1, r2, s1, s2);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.rangedAdd(audio1, r1, r2, s1, s2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.rangedAdd(audio1, r1, r2, s1, s2);
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
        else if (temp == "-cat"){
            soundFile1 = string(argv[i+1]);
            soundFile2 = string(argv[i+2]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.concatenate(audio1);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,int> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.concatenate(audio1);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int8_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.concatenate(audio1);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
                    audio1.loadToBuffer(soundFile2);
                    audio.concatenate(audio1);
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
        else if (temp == "-v"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> fr1 >> fr2;
            soundFile1 = string(argv[i+3]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.volume(fr1,fr2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.volume(fr1,fr2);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.volume(fr1,fr2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.volume(fr1,fr2);
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
        else if (temp == "-rev"){
            soundFile1 = string(argv[i+1]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.reverse();
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.reverse();
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.reverse();
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.reverse();
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
        else if (temp == "-rms"){
            soundFile1 = string(argv[i+1]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.rms();
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.rms();
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.rms();
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.rms();
                }
            }
            break;
        }
        else if (temp == "-norm"){
            temp = string(argv[i+1]) + " " + string(argv[i+2]);
            istringstream iss(temp);
            iss >> fr1 >> fr2;
            soundFile1 = string(argv[i+3]);
            if (channel == 1){
                if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.normalise(fr1, fr2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,int> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.normalise(fr1, fr2);
                    audio.writeToFile(outputFile);
                }
            }
            else{
                 if (bitSize == 8){
                    BRDAMY004::Audio<int8_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.normalise(fr1, fr2);
                    audio.writeToFile(outputFile);
                }
                else{
                    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
                    audio.loadToBuffer(soundFile1);
                    audio.normalise(fr1, fr2);
                    audio.writeToFile(outputFile);
                }
            }
            break;
        }
    }
    
    return 0;
}

