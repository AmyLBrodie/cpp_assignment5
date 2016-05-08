
/* 
 * Author: Amy Brodie (BRDAMY004)
 * CSC3022H Tutorial 5
 * Created on 06 May 2016, 2:53 PM
 */

#ifndef AUDIO_H
#define AUDIO_H
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
namespace BRDAMY004{
    template <typename T, int N=1>
    class Audio{
    private:
        std::vector<T> data_buffer;
        int sampleRate;
        int bitSize;
        int channel;
        int numberOfSamples;
        float seconds;
    public:
        Audio(int sRate, int bSize, int c);
        
        Audio(Audio & rhs);
        
        Audio(Audio && rhs);
        
        Audio & operator=(Audio & rhs);
        
        Audio & operator=(Audio && rhs);

        ~Audio();
        
        void loadToBuffer(std::string fileName);
        void writeToFile(std::string fileName);
        
        Audio & operator|(Audio & audio2);
        Audio & operator*(std::pair<float,float> vf);
        Audio & operator+(Audio & audio2);
        Audio & operator^(std::pair<int,int> cf);
        
        void add(Audio & audio2);
        void cut(int r1, int r2);
        void rangedAdd(Audio & audio2, int r1, int r2, int s1, int s2);
        void concatenate(Audio & audio2);
        void volume(float f1, float f2);
        void reverse();
        void rms();
        void normalise();
        
        
    };
    
    template <typename T, int N>
    Audio<T,N>::Audio(int sRate, int bSize, int c): sampleRate(sRate), bitSize(bSize), channel(c){
        
    }
    
    template <typename T, int N>
    Audio<T,N>::Audio(Audio<T,N> & rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds){
        for (int i=0; i<numberOfSamples; i++){
            data_buffer.push_back(rhs.data_buffer[i]);
        }
    }
    
    template <typename T, int N>
    Audio<T,N>::Audio(Audio<T,N> && rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds), data_buffer(std::move(rhs.data_buffer)){
        rhs.sampleRate = 0;
        rhs.numberOfSamples = 0;
        rhs.channel = 0;
        rhs.seconds = 0.0;
        rhs.bitSize = 0;
        rhs.data_buffer.clear();
    }
    
    template <typename T, int N>
    Audio<T,N> & Audio<T,N>::operator=(Audio<T,N> & rhs){
        //if (this != rhs){
            sampleRate = rhs.sampleRate;
            numberOfSamples = rhs.numberOfSamples;
            seconds = rhs.seconds;
            bitSize = rhs.bitSize;
            channel = rhs.channel;
            data_buffer.clear();
            for(int i=0; i<numberOfSamples; i++){
                data_buffer.push_back(rhs.data_buffer[i]);
            }   
        //}
        return *this;
    }
    
    template <typename T, int N>
    Audio<T,N> & Audio<T,N>::operator=(Audio<T,N> && rhs){
        if (*this != rhs){
            sampleRate = rhs.sampleRate;
            numberOfSamples = rhs.numberOfSamples;
            seconds = rhs.seconds;
            bitSize = rhs.bitSize;
            channel = rhs.channel;
            data_buffer = std::move(rhs.data_buffer);
            rhs.bitSize = 0;
            rhs.channel = 0;
            rhs.data_buffer.clear();
            rhs.numberOfSamples = 0;
            rhs.sampleRate = 0;
            rhs.seconds = 0.0;
        }
        return *this;
    }
    
    template <typename T,int N>
    void Audio<T,N>::loadToBuffer(std::string fileName){
        int fileSize;
        std::ifstream stream(fileName, std::ios::binary);
        
        stream.seekg(0, stream.end);
        fileSize = stream.tellg();
        stream.seekg(0, stream.beg);
        
        numberOfSamples = fileSize / (sizeof(T) * channel);
        seconds = numberOfSamples/(float) sampleRate;
        
        data_buffer.resize(numberOfSamples);
        
        stream.read((char *)&data_buffer[0], numberOfSamples);
        
        stream.close();
        
        std::cout << fileSize << ":" << seconds << std::endl;
    }
    
    template <typename T,int N>
    void Audio<T,N>::writeToFile(std::string fileName){
        std::ofstream stream(fileName, std::ios::binary);
        
        stream.write((char *)&data_buffer[0], Audio::numberOfSamples);
        
        stream.close();
    }
    
    template <typename T, int N>
     Audio<T,N> & Audio<T,N>::operator|(Audio<T,N> & audio2){
        numberOfSamples = numberOfSamples+audio2.numberOfSamples;
        for (typename std::vector<T>::iterator i=audio2.data_buffer.begin(); i!=audio2.data_buffer.end(); i++){
            data_buffer.push_back(*i);
        }
        return *this;
    }
    
    template <typename T, int N>
    Audio<T,N> & Audio<T,N>::operator*(std::pair<float,float> vf){
        if (channel == 1){
            for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                *i = *i * vf.first;
            }
        }
        
        return *this;
    }
    
    template <typename T, int N>
    Audio<T,N> & Audio<T,N>::operator+(Audio<T,N> & audio2){
        typename std::vector<T>::iterator it2 = audio2.data_buffer.begin();
        for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
            T temp = *i + *it2;
            if (temp > std::numeric_limits<T>::max()){
                temp = std::numeric_limits<T>::max();
            }
            else  if (temp < std::numeric_limits<T>::min()){
                temp = std::numeric_limits<T>::min();
            }
            *i = temp;
            it2++;
        }
        
        return *this;
    }
    
    template <typename T, int N>
    Audio<T,N> & Audio<T,N>::operator^(std::pair<int,int> cf){
        std::vector<T> newBuffer;
        for(int i=cf.first; i<cf.second; i++){
            newBuffer.push_back(data_buffer[i]);
        }
        data_buffer.clear();
        data_buffer = newBuffer;
        numberOfSamples = cf.second - cf.first;
        
        return *this;
    }
    
    template <typename T,int N>
    void Audio<T,N>::add(Audio<T,N> & audio2){
        *this = *this + audio2;
    }
    
    template <typename T,int N>
    void Audio<T,N>::concatenate(Audio<T,N> & audio2){
        *this =  *this | audio2;
    }
    
    template <typename T,int N>
    void Audio<T,N>::cut(int r1, int r2){
        std::pair<int,int> range(r1,r2);
        *this = *this^range;
    }
    
    template <typename T,int N>
    void Audio<T,N>::volume(float f1, float f2){
        std::pair<float,float> factor(f1,f2);
        *this = *this * factor;
    }
    
    template <typename T,int N>
    void Audio<T,N>::reverse(){
        std::reverse(data_buffer.begin(), data_buffer.end());
    }
    
    template <typename T,int N>
    void Audio<T,N>::rangedAdd(Audio& audio2, int r1, int r2, int s1, int s2){
        if (r1-r2 == s1-s2){
            r1 = r1*sampleRate;
            r2 = r2*sampleRate;
            s1 = s1*sampleRate;
            s2 = s2*sampleRate;
            std::cout << r1 << ":" << r2 << ":" << s1 << ":" << s2 << std::endl;
            std::vector<T> newBuffer1(r2-r1), newBuffer2(s2-s1);
            std::copy(data_buffer.begin()+r1, data_buffer.begin()+r2, newBuffer1.begin());
            std::copy(audio2.data_buffer.begin()+s1, audio2.data_buffer.begin()+s2, newBuffer2.begin());
            data_buffer = newBuffer1;
            numberOfSamples = data_buffer.size();
            audio2.data_buffer = newBuffer2;
            numberOfSamples = audio2.data_buffer.size();
            *this = *this + audio2;
        }
        
    }
    
    template <typename T,int N>
    Audio<T,N>::~Audio(){
        
    }
    
    
    template <typename T>
    
    class Audio<T,2>{
    private:
        std::vector<std::pair<T,T>> data_buffer;
        int sampleRate;
        int bitSize;
        int channel;
        int numberOfSamples;
        float seconds;
    public:
        void loadToBuffer(std::string fileName){
            int fileSize;
            std::ifstream stream(fileName, std::ios::binary);

            stream.seekg(0, stream.end);
            fileSize = stream.tellg();
            stream.seekg(0, stream.beg);

            numberOfSamples = fileSize / (sizeof(T) * channel);
            seconds = numberOfSamples/(float) sampleRate;
            int tempSamples = fileSize / (sizeof(T) * 1);
            std::vector<T> tempVector(tempSamples);

            stream.read((char *)&tempVector[0], numberOfSamples);

            stream.close();
            std::pair<T,T> tempPair;
            for (int i=0; i<tempSamples; i++){
                if (i%2==0){
                    tempPair.first = tempVector[i];
                }
                else{
                    tempPair.second = tempVector[i];
                    data_buffer.push_back(tempPair);
                }
            }
        }
        
        void writeToFile(std::string fileName){
            
            int fileSize = numberOfSamples * (sizeof(T) * channel);
            int tempSamples = fileSize/(sizeof(T) * 1);
            std::vector<T> tempVector;
            
            for (int i=0; i<numberOfSamples; i++){
                tempVector.push_back(data_buffer[i].first);
                tempVector.push_back(data_buffer[i].second);
            }
            
            std::ofstream stream(fileName, std::ios::binary);
            
            stream.write((char *)&tempVector[0], tempSamples);
        
            stream.close();
        }
        
        Audio & operator|(Audio & audio2){
            
        }
        
        Audio & operator*(std::pair<float,float> vf){
            
        }
        
        Audio & operator+(Audio & audio2){
            
        }
        
        Audio & operator^(std::pair<int,int> cf){
            
        }
        
        void add(Audio & audio2);
        void cut(int r1, int r2);
        void rangedAdd(Audio & audio2, int r1, int r2, int s1, int s2);
        void concatenate(Audio & audio2);
        void volume(float f1, float f2);
        void reverse();
        void rms();
        void normalise();
            
    };
    
    
}


#endif /* AUDIO_H */

