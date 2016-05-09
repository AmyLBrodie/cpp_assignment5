#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Audio.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

TEST_CASE("Move Constructor for Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    std::vector<std::pair<int16_t,int16_t>> original;
    for (int i=0; i<audio.getBuffer().size(); i++){
        original.push_back(audio.getBuffer()[i]);
    }
    BRDAMY004::Audio<int16_t,float> audio1(std::move(audio));
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer1.size() == 0);
    for (int i=0; i<data_buffer2.size(); i++){
        REQUIRE(data_buffer2[i].first == original[i].first);
        REQUIRE(data_buffer2[i].second == original[i].second);
    }
}

TEST_CASE("Copy Constructor for Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    BRDAMY004::Audio<int16_t,float> audio1(audio);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer1.size() != 0);
    REQUIRE(data_buffer1.size() == data_buffer2.size());
    for (int i=0; i<data_buffer2.size(); i++){
        REQUIRE(data_buffer2[i].first == data_buffer1[i].first);
        REQUIRE(data_buffer2[i].second == data_buffer1[i].second);
    }
}

TEST_CASE("Copy Assignment Operator for Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
    audio1.loadToBuffer(soundFile2);
    audio = audio1;
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer1.size() != 0);
    REQUIRE(data_buffer1.size() == data_buffer2.size());
    for (int i=0; i<data_buffer2.size(); i++){
        REQUIRE(data_buffer2[i].first == data_buffer1[i].first);
        REQUIRE(data_buffer2[i].second == data_buffer1[i].second);
    }
}

TEST_CASE("Move Assignment Operator for Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
    audio1.loadToBuffer(soundFile2);
    std::vector<std::pair<int16_t,int16_t>> original;
    for (int i=0; i<audio1.getBuffer().size(); i++){
        original.push_back(audio1.getBuffer()[i]);
    }
    audio = std::move(audio1);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer2.size() == 0);
    for (int i=0; i<data_buffer2.size(); i++){
        REQUIRE(data_buffer1[i].first == original[i].first);
        REQUIRE(data_buffer1[i].second == original[i].second);
    }
}

TEST_CASE("The Concatenation Operator |"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
    audio1.loadToBuffer(soundFile2);
    audio.concatenate(audio1);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer1.size() == 20);
    REQUIRE((int)data_buffer1[0].first == -2152);
    REQUIRE((int)data_buffer1[0].second == -3641);
    REQUIRE((int)data_buffer1[9].first == -147);
    REQUIRE((int)data_buffer1[9].second == -1906);
    REQUIRE((int)data_buffer1[10].first == -993);
    REQUIRE((int)data_buffer1[10].second == -953);
    REQUIRE((int)data_buffer1[19].first == -2673);
    REQUIRE((int)data_buffer1[19].second == -3527);
}

TEST_CASE("The Addition Operator +"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
    audio1.loadToBuffer(soundFile2);
    audio.add(audio1);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    
    REQUIRE(data_buffer1.size() == 10);
    REQUIRE((int)data_buffer1[0].first == -3145);
    REQUIRE((int)data_buffer1[0].second == -4594);
    REQUIRE((int)data_buffer1[1].first == -4009);
    REQUIRE((int)data_buffer1[1].second == -2986);
    REQUIRE((int)data_buffer1[8].first == -5861);
    REQUIRE((int)data_buffer1[8].second == -6162);
    REQUIRE((int)data_buffer1[9].first == -2820);
    REQUIRE((int)data_buffer1[9].second == -5433);
}

TEST_CASE("The Cut Operator ^"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    audio.cut(0,5);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1;
    data_buffer1 = audio.getBuffer();
    
    REQUIRE(data_buffer1.size() == 5);
    REQUIRE((int)data_buffer1[0].first == -2152);
    REQUIRE((int)data_buffer1[0].second == -3641);
    REQUIRE((int)data_buffer1[1].first == -2538);
    REQUIRE((int)data_buffer1[1].second == -3152);
    REQUIRE((int)data_buffer1[3].first == -752);
    REQUIRE((int)data_buffer1[3].second == -2391);
    REQUIRE((int)data_buffer1[4].first == -1242);
    REQUIRE((int)data_buffer1[4].second == -3001);
}

TEST_CASE("The Volume Factor Operator *"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    audio.volume(0.75,0.5);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1;
    data_buffer1 = audio.getBuffer();
    
    REQUIRE(data_buffer1.size() == 10);
    REQUIRE((int)data_buffer1[0].first == -1614);
    REQUIRE((int)data_buffer1[0].second == -1820);
    REQUIRE((int)data_buffer1[1].first == -1903);
    REQUIRE((int)data_buffer1[1].second == -1576);
    REQUIRE((int)data_buffer1[8].first == -1417);
    REQUIRE((int)data_buffer1[8].second == -1238);
    REQUIRE((int)data_buffer1[9].first == -110);
    REQUIRE((int)data_buffer1[9].second == -953);
}


TEST_CASE("Reverse Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile1);
    audio.reverse();
    std::vector<std::pair<int16_t,int16_t>> data_buffer1;
    data_buffer1 = audio.getBuffer();
    
    REQUIRE(data_buffer1.size() == 10);
    REQUIRE((int)data_buffer1[0].first == -147);
    REQUIRE((int)data_buffer1[0].second == -1906);
    REQUIRE((int)data_buffer1[1].first == -1890);
    REQUIRE((int)data_buffer1[1].second == -2476);
    REQUIRE((int)data_buffer1[8].first == -2538);
    REQUIRE((int)data_buffer1[8].second == -3152);
    REQUIRE((int)data_buffer1[9].first == -2152);
    REQUIRE((int)data_buffer1[9].second == -3641);
}

TEST_CASE("Ranged Add"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile3);
    BRDAMY004::Audio<int16_t,float> audio1(sampleRate, bitSize, channel);
    audio1.loadToBuffer(soundFile3);
    audio.rangedAdd(audio1,0,1,0,1);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1, data_buffer2;
    data_buffer1 = audio.getBuffer();
    data_buffer2 = audio1.getBuffer();
    int size = data_buffer1.size();
    
    REQUIRE((int)data_buffer1[size-1].first == 6522);
    REQUIRE((int)data_buffer1[size-1].second == 5270);
    REQUIRE((int)data_buffer1[size-2].first == 6862);
    REQUIRE((int)data_buffer1[size-2].second == 5242);
    REQUIRE((int)data_buffer1[size-3].first == 5892);
    REQUIRE((int)data_buffer1[size-3].second == 4778);
    REQUIRE((int)data_buffer1[size-4].first == 4840);
    REQUIRE((int)data_buffer1[size-4].second == 3018);
}

TEST_CASE("Compute RMS"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile3);
    std::pair<float,float> rmsPair = audio.calculateRMS();
    float one = 21.7244f;
    float two = 27.1885f;
    REQUIRE((int)rmsPair.first == (int) one);
    REQUIRE((int)rmsPair.second == (int) two);
}

TEST_CASE("Nomalise Audio"){
    int sampleRate = 44100;
    int bitSize = 16;
    int channel = 2;
    std::string soundFile1 = "test1_44100_16_stereo.raw";
    std::string soundFile2 = "test2_44100_16_stereo.raw";
    std::string soundFile3 = "test3_44100_16_stereo.raw";
    BRDAMY004::Audio<int16_t,float> audio(sampleRate, bitSize, channel);
    audio.loadToBuffer(soundFile3);
    audio.normalise(15.0f, 12.0f);
    std::vector<std::pair<int16_t,int16_t>> data_buffer1;
    data_buffer1 = audio.getBuffer();
    int size = data_buffer1.size();
    
    REQUIRE((int)data_buffer1[size-1].first == 2251);
    REQUIRE((int)data_buffer1[size-1].second == 1162);
    REQUIRE((int)data_buffer1[size-2].first == 2368);
    REQUIRE((int)data_buffer1[size-2].second == 1156);
    REQUIRE((int)data_buffer1[size-3].first == 2034);
    REQUIRE((int)data_buffer1[size-3].second == 1054);
    REQUIRE((int)data_buffer1[size-4].first == 1670);
    REQUIRE((int)data_buffer1[size-4].second == 666);
}

