
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
#include <cmath>
namespace BRDAMY004{
    template <typename T, typename N>
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
        
        // Functor for normalization of sound files -- mono
        class normal{
        private:
            float current, desired;
        public:
            normal(float c_rms=0.0f, float rms=0.0f) : current(c_rms), desired(rms){
                
            }
            
            T operator()(T & input){
                T temp = input * (desired/current);
                if (temp > std::numeric_limits<T>::max()){
                    temp = std::numeric_limits<T>::max();
                }
                else  if (temp < std::numeric_limits<T>::min()){
                    temp = std::numeric_limits<T>::min();
                }
                return temp;
            }
        };
        
        
        void add(Audio & audio2);
        void cut(int r1, int r2);
        void rangedAdd(Audio & audio2, int r1, int r2, int s1, int s2);
        void concatenate(Audio & audio2);
        void volume(float f1, float f2);
        void reverse();
        void rms();
        void normalise(float r1, float r2);
        
        float calculateRMS();
        std::vector<T> getBuffer();
    };
    
    // Default constructor -- mono
    template <typename T, typename N>
    Audio<T,N>::Audio(int sRate, int bSize, int c): sampleRate(sRate), bitSize(bSize), channel(c){
        
    }
    
    //copy constructor -- mono
    template <typename T, typename N>
    Audio<T,N>::Audio(Audio<T,N> & rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds){
        for (int i=0; i<numberOfSamples; i++){
            data_buffer.push_back(rhs.data_buffer[i]);
        }
    }
    
    // move constructor -- mono
    template <typename T, typename N>
    Audio<T,N>::Audio(Audio<T,N> && rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds), data_buffer(std::move(rhs.data_buffer)){
        rhs.sampleRate = 0;
        rhs.numberOfSamples = 0;
        rhs.channel = 0;
        rhs.seconds = 0.0;
        rhs.bitSize = 0;
        rhs.data_buffer.clear();
    }
    
    // copy assignment operator -- mono
    template <typename T, typename N>
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
    
    // move assignment operator -- mono
    template <typename T, typename N>
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
    
    // Loads a .raw file into the vector of the Audio template -- mono
    template <typename T,typename N>
    void Audio<T,N>::loadToBuffer(std::string fileName){
        int fileSize;
        std::ifstream stream(fileName, std::ios::binary);
        
        stream.seekg(0, stream.end);
        fileSize = stream.tellg();
        stream.seekg(0, stream.beg);
        
        numberOfSamples = fileSize / (sizeof(T) * channel);
        seconds = numberOfSamples/(float) sampleRate;
        
        data_buffer.resize(fileSize);
        
        stream.read((char *)&data_buffer[0], fileSize);
        
        data_buffer.resize(numberOfSamples);
        stream.close();
    }
    
    // Writes the contents of the vector of the audio template into a .raw file -- mono
    template <typename T,typename N>
    void Audio<T,N>::writeToFile(std::string fileName){
        
        std::ostringstream oss;
        oss << sampleRate;
        fileName += "_" + oss.str();
        std::ostringstream oss1;
        oss1 << bitSize;
        fileName += "_" + oss1.str() + "_mono.raw";
        
        int fileSize = numberOfSamples * (sizeof(T) * channel);
        
        std::ofstream stream(fileName, std::ios::binary);
        
        stream.write((char *)&data_buffer[0], fileSize);
        
        stream.close();
    }
    
    // operator overload for concatenation -- mono
    template <typename T, typename N>
     Audio<T,N> & Audio<T,N>::operator|(Audio<T,N> & audio2){
        numberOfSamples = numberOfSamples+audio2.numberOfSamples;
        for (typename std::vector<T>::iterator i=audio2.data_buffer.begin(); i!=audio2.data_buffer.end(); i++){
            data_buffer.push_back(*i);
        }
        return *this;
    }
    
    // operator overload for volume factor -- mono
    template <typename T, typename N>
    Audio<T,N> & Audio<T,N>::operator*(std::pair<float,float> vf){
        if (channel == 1){
            for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                *i = *i * vf.first;
            }
        }
        
        return *this;
    }
    
    // operator overload for addition of sound files -- mono
    template <typename T, typename N>
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
    
    // operator overload for cutting of sound file -- mono
    template <typename T, typename N>
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
    
    // adds two sound files together -- mono
    template <typename T,typename N>
    void Audio<T,N>::add(Audio<T,N> & audio2){
        *this = *this + audio2;
    }
    
    // concatenates two sound files -- mono
    template <typename T,typename N>
    void Audio<T,N>::concatenate(Audio<T,N> & audio2){
        *this =  *this | audio2;
    }
    
    // cuts a sound file based on range supplied -- mono
    template <typename T,typename N>
    void Audio<T,N>::cut(int r1, int r2){
        std::pair<int,int> range(r1,r2);
        *this = *this^range;
    }
    
    // adjusts volume of sound file based on factor -- mono
    template <typename T,typename N>
    void Audio<T,N>::volume(float f1, float f2){
        std::pair<float,float> factor(f1,f2);
        *this = *this * factor;
    }
    
    // reverses sound file -- mono
    template <typename T,typename N>
    void Audio<T,N>::reverse(){
        std::reverse(data_buffer.begin(), data_buffer.end());
    }
    
    // adds specified ranges of sound files -- mono
    template <typename T,typename N>
    void Audio<T,N>::rangedAdd(Audio& audio2, int r1, int r2, int s1, int s2){
        if (r1-r2 == s1-s2){
            r1 = r1*sampleRate;
            r2 = r2*sampleRate;
            s1 = s1*sampleRate;
            s2 = s2*sampleRate;
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
    
    // calculates and prints out rms using a lambda -- mono
    template <typename T,typename N>
    void Audio<T,N>::rms(){
        std::vector<T> squareVector;
        for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
            squareVector.push_back((*i)*(*i));
        }
        int squareSum = std::accumulate(squareVector.begin(), squareVector.end(), 0);
        int m = numberOfSamples;
        auto compute = [this, squareSum] (int n) -> float {return sqrt(squareSum/(float)n);};
        
        auto rms = compute(m);
        std::cout << "The RMS value of the audio file is: ";
        std::cout << rms;
        std::cout << " " << std::endl;
    }
    
    // calculates rms for testing -- mono
    template <typename T,typename N>
    float Audio<T,N>::calculateRMS(){
        std::vector<T> squareVector;
        for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
            squareVector.push_back((*i)*(*i));
        }
        int squareSum = std::accumulate(squareVector.begin(), squareVector.end(), 0);
        float c_rms = sqrt(squareSum/(float)numberOfSamples);
        return c_rms;
    }
    
    // normalises a sound file with given rms values -- mono
    template <typename T,typename N>
    void Audio<T,N>::normalise(float r1, float r2){
        std::vector<T> squareVector;
        for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
            squareVector.push_back((*i)*(*i));
        }
        int squareSum = std::accumulate(squareVector.begin(), squareVector.end(), 0);
        float c_rms = sqrt(squareSum/(float)numberOfSamples);
        Audio::normal norm(r1, c_rms);
        std::transform(data_buffer.begin(), data_buffer.end(), data_buffer.begin(), norm);
    }
    
    // returns data_buffer for audio template -- mono
    template <typename T,typename N>
    std::vector<T> Audio<T,N>::getBuffer(){
        return data_buffer;
    }
    
    // destructor -- mono
    template <typename T,typename N>
    Audio<T,N>::~Audio(){
        
    }
    
    
    
    // Specialization for stereo
    template <typename T>
    class Audio<T, float>{
    private:
        std::vector<std::pair<T,T>> data_buffer;
        int sampleRate;
        int bitSize;
        int channel;
        int numberOfSamples;
        float seconds;
    public:
        // Default constructor -- stereo
        Audio(int sRate, int bSize, int c): sampleRate(sRate), bitSize(bSize), channel(c){
            
        }
        
        // copy constructor -- stereo
        Audio(Audio & rhs): sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds){
            for (int i=0; i<numberOfSamples; i++){
                data_buffer.push_back(rhs.data_buffer[i]);
            }
        }
        
        // move constructor -- stereo
        Audio(Audio && rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds), data_buffer(std::move(rhs.data_buffer)){
            rhs.sampleRate = 0;
            rhs.numberOfSamples = 0;
            rhs.channel = 0;
            rhs.seconds = 0.0;
            rhs.bitSize = 0;
            rhs.data_buffer.clear();
        }
        
        // copy assignment operator -- stereo
        Audio & operator=(Audio & rhs){
            if (this != &rhs){
                sampleRate = rhs.sampleRate;
                numberOfSamples = rhs.numberOfSamples;
                seconds = rhs.seconds;
                bitSize = rhs.bitSize;
                channel = rhs.channel;
                data_buffer.clear();
                for(int i=0; i<numberOfSamples; i++){
                    data_buffer.push_back(rhs.data_buffer[i]);
                }   
            }
            return *this;
        }
        
        // move assignment operator -- stereo
        Audio & operator=(Audio && rhs){
            if (this != &rhs){
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
        
        // returns data_buffer for audio template -- stereo
        ~Audio(){
            
        }
        
        // Loads a .raw file into the vector of the Audio template -- stereo
        void loadToBuffer(std::string fileName){
            int fileSize;
            std::ifstream stream(fileName, std::ios::binary);

            stream.seekg(0, stream.end);
            fileSize = stream.tellg();
            stream.seekg(0, stream.beg);

            numberOfSamples = fileSize / (sizeof(T) * channel);
            seconds = numberOfSamples/(float) sampleRate;
            int tempSamples = fileSize;
            std::vector<T> tempVector(tempSamples);

            stream.read((char *)&tempVector[0], tempSamples);

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
            data_buffer.resize(numberOfSamples);
        }
        
        // Writes the contents of the vector of the audio template into a .raw file -- stereo
        void writeToFile(std::string fileName){
            
            std::ostringstream oss;
            oss << sampleRate;
            fileName += "_" + oss.str();
            std::ostringstream oss1;
            oss1 << bitSize;
            fileName += "_" + oss1.str() + "_stereo.raw";
            
            int fileSize = numberOfSamples * (sizeof(T) * channel);
            int tempSamples = fileSize;
            std::vector<T> tempVector;
            
            for (typename std::vector<std::pair<T,T>>::iterator i = data_buffer.begin(); i!=data_buffer.end(); i++){
                tempVector.push_back(i->first);
                tempVector.push_back(i->second);
            }
            
            std::ofstream stream(fileName.c_str(), std::ios::binary);
            
            stream.write((char *)&tempVector[0], tempSamples);
        
            stream.close();
        }
        
        // Functor for normalization of sound files -- stereo
        class normal{
        private:
            float current1, current2, desired1, desired2;
        public:
            normal(float c_rms1=0.0f, float c_rms2=0.0f, float rms1=0.0f, float rms2=0.0f) : current1(c_rms1), current2(c_rms2), desired1(rms1), desired2(rms2){
                
            }
            
            std::pair<T,T> operator()(std::pair<T,T> & input){
                std::pair<T,T> temp;
                temp.first = input.first * (desired1/current1);
                temp.second = input.second * (desired2/current2);
                if (temp.first > std::numeric_limits<T>::max()){
                    temp.first = std::numeric_limits<T>::max();
                }
                else  if (temp.first < std::numeric_limits<T>::min()){
                    temp.first = std::numeric_limits<T>::min();
                }
                if (temp.second > std::numeric_limits<T>::max()){
                    temp.second = std::numeric_limits<T>::max();
                }
                else  if (temp.second < std::numeric_limits<T>::min()){
                    temp.second = std::numeric_limits<T>::min();
                }
                return temp;
            }
        };
        
        // operator overload for concatenation -- stereo
        Audio & operator|(Audio & audio2){
            numberOfSamples = numberOfSamples+audio2.numberOfSamples;
            for (typename std::vector<std::pair<T,T>>::iterator i=audio2.data_buffer.begin(); i!=audio2.data_buffer.end(); i++){
                data_buffer.push_back(*i);
            }
            return *this;
        }
        
        // operator overload for volume factor -- stereo
        Audio & operator*(std::pair<float,float> vf){
            if (channel == 2){
                for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                    i->first = i->first * vf.first;
                    i->second = i->second * vf.second;
                }
            }
            return *this;
        }
        
        // operator overload for addition of sound file -- stereo
        Audio & operator+(Audio & audio2){
            typename std::vector<std::pair<T,T>>::iterator it2 = audio2.data_buffer.begin();
            for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                T temp1 = i->first + it2->first;
                T temp2 = i->second + it2->second;
                if (temp1 > std::numeric_limits<T>::max()){
                    temp1 = std::numeric_limits<T>::max();
                }
                else  if (temp1 < std::numeric_limits<T>::min()){
                    temp1 = std::numeric_limits<T>::min();
                }
                if (temp2 > std::numeric_limits<T>::max()){
                    temp2 = std::numeric_limits<T>::max();
                }
                else  if (temp2 < std::numeric_limits<T>::min()){
                    temp2 = std::numeric_limits<T>::min();
                }
                i->first = temp1;
                i->second = temp2;
                it2++;
            }

            return *this;
        }
        
        // operator overload for cutting of sound files -- stereo
        Audio & operator^(std::pair<int,int> cf){
            std::vector<std::pair<T,T>> newBuffer;
            for(int i=cf.first; i<cf.second; i++){
                newBuffer.push_back(data_buffer[i]);
            }
            data_buffer.clear();
            data_buffer = newBuffer;
            numberOfSamples = cf.second - cf.first;

            return *this;
        }
        
        // addition of two sound files -- stereo
        void add(Audio & audio2){
            *this = *this + audio2;
        }
        
        // cutting of sound file based on supplied range -- stereo
        void cut(int r1, int r2){
            std::pair<int,int> range(r1,r2);
            *this = *this^range;
        }
        
        // addition of sound files based on supplied ranges -- stereo
        void rangedAdd(Audio & audio2, int r1, int r2, int s1, int s2){
            if (r1-r2 == s1-s2){
                r1 = r1*sampleRate;
                r2 = r2*sampleRate;
                s1 = s1*sampleRate;
                s2 = s2*sampleRate;
                std::vector<std::pair<T,T>> newBuffer1(r2-r1), newBuffer2(s2-s1);
                std::copy(data_buffer.begin()+r1, data_buffer.begin()+r2, newBuffer1.begin());
                std::copy(audio2.data_buffer.begin()+s1, audio2.data_buffer.begin()+s2, newBuffer2.begin());
                data_buffer = newBuffer1;
                numberOfSamples = data_buffer.size();
                audio2.data_buffer = newBuffer2;
                numberOfSamples = audio2.data_buffer.size();
                *this = *this + audio2;
            }
        }
        
        // concatenation of two sound files -- stereo
        void concatenate(Audio & audio2){
            *this =  *this | audio2;
        }
        
        // volume adjustment of sound file based on volume factor -- stereo
        void volume(float f1, float f2){
            std::pair<float,float> factor(f1,f2);
            *this = *this * factor;
        }
        
        // reversal of sound file -- stereo
        void reverse(){
            std::vector<T> temp_buffer;
            for (int i=0; i<numberOfSamples; i++){
                temp_buffer.push_back(data_buffer[i].first);
                temp_buffer.push_back(data_buffer[i].second);
            }
            std::reverse(temp_buffer.begin(), temp_buffer.end());
            std::pair<T,T> tempPair;
            std::vector<std::pair<T,T>> temp_buffer2;
            for (int i=0; i<temp_buffer.size(); i++){
                if (i%2==0){
                    tempPair.second = temp_buffer[i];
                }
                else{
                    tempPair.first = temp_buffer[i];
                    temp_buffer2.push_back(tempPair);
                }
            }
            data_buffer = temp_buffer2;
        }
        
        // calculation of rms value using a lambda -- stereo
        void rms(){
            std::vector<T> squareVector1, squareVector2;
            for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                squareVector1.push_back((i->first)*(i->first));
                squareVector2.push_back((i->second)*(i->second));
            }
            int squareSum1 = std::accumulate(squareVector1.begin(), squareVector1.end(), 0);
            int squareSum2 = std::accumulate(squareVector2.begin(), squareVector2.end(), 0);
            int m = numberOfSamples;
            auto compute = [this, m] (int squareSum) -> float {return sqrt(squareSum/(float)m);};
            
            auto left_rms = compute(squareSum1);
            auto right_rms = compute(squareSum2);
            std::cout << "The RMS value of the left channel of the audio file is: ";
            std::cout << left_rms;
            std::cout << " " << std::endl;
            std::cout << "The RMS value of the right channel of the audio file is: ";
            std::cout << right_rms;
            std::cout << " " << std::endl;
        }
        
        // calculation of rms for testing -- stereo
        std::pair<float,float> calculateRMS(){
            std::vector<T> squareVector1, squareVector2;
            for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                squareVector1.push_back((i->first)*(i->first));
                squareVector2.push_back((i->second)*(i->second));
            }
            int squareSum1 = std::accumulate(squareVector1.begin(), squareVector1.end(), 0);
            int squareSum2 = std::accumulate(squareVector2.begin(), squareVector2.end(), 0);
            float c_rms1 = sqrt(squareSum1/(float)numberOfSamples);
            float c_rms2 = sqrt(squareSum2/(float)numberOfSamples);
            return std::make_pair(c_rms1,c_rms2);
        }
        
        // normalisation of sound file based on supplied rms values -- stereo
        void normalise(float r1, float r2){
            std::vector<T> squareVector1, squareVector2;
            for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                squareVector1.push_back((i->first)*(i->first));
                squareVector2.push_back((i->second)*(i->second));
            }
            int squareSum1 = std::accumulate(squareVector1.begin(), squareVector1.end(), 0);
            int squareSum2 = std::accumulate(squareVector2.begin(), squareVector2.end(), 0);
            float c_rms1 = sqrt(squareSum1/(float)numberOfSamples);
            float c_rms2 = sqrt(squareSum2/(float)numberOfSamples);
            Audio::normal norm(c_rms1, c_rms2, r1, r2);
            std::transform(data_buffer.begin(), data_buffer.end(), data_buffer.begin(), norm);
            
        }
        
        // returns the data_buffer for testing -- stereo
        std::vector<std::pair<T,T>> getBuffer(){
            return data_buffer;
        }
            
    };
    
    
}


#endif /* AUDIO_H */

