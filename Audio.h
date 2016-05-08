
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
                //std::cout << (int)temp << " ";
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
        
        
    };
    
    template <typename T, typename N>
    Audio<T,N>::Audio(int sRate, int bSize, int c): sampleRate(sRate), bitSize(bSize), channel(c){
        
    }
    
    template <typename T, typename N>
    Audio<T,N>::Audio(Audio<T,N> & rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds){
        for (int i=0; i<numberOfSamples; i++){
            data_buffer.push_back(rhs.data_buffer[i]);
        }
    }
    
    template <typename T, typename N>
    Audio<T,N>::Audio(Audio<T,N> && rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds), data_buffer(std::move(rhs.data_buffer)){
        rhs.sampleRate = 0;
        rhs.numberOfSamples = 0;
        rhs.channel = 0;
        rhs.seconds = 0.0;
        rhs.bitSize = 0;
        rhs.data_buffer.clear();
    }
    
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
    
    template <typename T,typename N>
    void Audio<T,N>::loadToBuffer(std::string fileName){
        std::cout << "here" << std::endl;
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
    
    template <typename T,typename N>
    void Audio<T,N>::writeToFile(std::string fileName){
        std::ofstream stream(fileName, std::ios::binary);
        
        stream.write((char *)&data_buffer[0], Audio::numberOfSamples);
        
        stream.close();
    }
    
    template <typename T, typename N>
     Audio<T,N> & Audio<T,N>::operator|(Audio<T,N> & audio2){
        numberOfSamples = numberOfSamples+audio2.numberOfSamples;
        for (typename std::vector<T>::iterator i=audio2.data_buffer.begin(); i!=audio2.data_buffer.end(); i++){
            data_buffer.push_back(*i);
        }
        return *this;
    }
    
    template <typename T, typename N>
    Audio<T,N> & Audio<T,N>::operator*(std::pair<float,float> vf){
        if (channel == 1){
            for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                *i = *i * vf.first;
            }
        }
        
        return *this;
    }
    
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
    
    template <typename T,typename N>
    void Audio<T,N>::add(Audio<T,N> & audio2){
        *this = *this + audio2;
    }
    
    template <typename T,typename N>
    void Audio<T,N>::concatenate(Audio<T,N> & audio2){
        *this =  *this | audio2;
    }
    
    template <typename T,typename N>
    void Audio<T,N>::cut(int r1, int r2){
        std::pair<int,int> range(r1,r2);
        *this = *this^range;
    }
    
    template <typename T,typename N>
    void Audio<T,N>::volume(float f1, float f2){
        std::pair<float,float> factor(f1,f2);
        *this = *this * factor;
    }
    
    template <typename T,typename N>
    void Audio<T,N>::reverse(){
        std::reverse(data_buffer.begin(), data_buffer.end());
    }
    
    template <typename T,typename N>
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
    
    template <typename T,typename N>
    void Audio<T,N>::rms(){
        std::cout <<":::"<< std::endl;
        std::vector<T> squareVector;
        for (typename std::vector<T>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
            squareVector.push_back((*i)*(*i));
        }
        int squareSum = std::accumulate(squareVector.begin(), squareVector.end(), 0);
        int m = numberOfSamples;
        auto compute = [this, squareSum] (int n) -> float {return sqrt(squareSum/(float)n);};
        
        auto rms = compute(m);
        std::cout << "The RMS value of the audio file is: ";
        //std::cout << sqrt(squareSum/(float)m) << std::endl;
        std::cout << rms;
        std::cout << " " << std::endl;
    }
    
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
    
    template <typename T,typename N>
    Audio<T,N>::~Audio(){
        
    }
    
    
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
        Audio(int sRate, int bSize, int c): sampleRate(sRate), bitSize(bSize), channel(c){
            
        }
        
        Audio(Audio & rhs): sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds){
            for (int i=0; i<numberOfSamples; i++){
                data_buffer.push_back(rhs.data_buffer[i]);
            }
        }
        
        Audio(Audio && rhs) : sampleRate(rhs.sampleRate), bitSize(rhs.bitSize), channel(rhs.channel), numberOfSamples(rhs.numberOfSamples), seconds(rhs.seconds), data_buffer(std::move(rhs.data_buffer)){
            rhs.sampleRate = 0;
            rhs.numberOfSamples = 0;
            rhs.channel = 0;
            rhs.seconds = 0.0;
            rhs.bitSize = 0;
            rhs.data_buffer.clear();
        }
        
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

        ~Audio(){
            
        }
        
        void loadToBuffer(std::string fileName){
            std::cout << "here1" << std::endl;
            int fileSize;
            std::ifstream stream(fileName, std::ios::binary);

            stream.seekg(0, stream.end);
            fileSize = stream.tellg();
            stream.seekg(0, stream.beg);

            numberOfSamples = fileSize / (sizeof(T) * channel);
            seconds = numberOfSamples/(float) sampleRate;
            int tempSamples = fileSize / (sizeof(T) * 1);
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
            std::cout << fileSize << ":" << seconds << std::endl;
        }
        
        void writeToFile(std::string fileName){
            
            int fileSize = numberOfSamples * (sizeof(T) * channel);
            int tempSamples = fileSize/(sizeof(T) * 1);
            std::vector<T> tempVector;
            std::cout << fileSize << ":" << numberOfSamples << std::endl;
            
            for (typename std::vector<std::pair<T,T>>::iterator i = data_buffer.begin(); i!=data_buffer.end(); i++){
                tempVector.push_back(i->first);
                tempVector.push_back(i->second);
            }
            
            std::ofstream stream(fileName, std::ios::binary);
            
            stream.write((char *)&tempVector[0], tempSamples);
        
            stream.close();
        }
        
        class normal{
        private:
            float current1, current2, desired1, desired2;
        public:
            normal(float c_rms1=0.0f, float c_rms2=0.0f, float rms1=0.0f, float rms2=0.0f) : current1(c_rms1), current2(c_rms2), desired1(rms), desired2(rms2){
                
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
                //std::cout << (int)temp << " ";
                return temp;
            }
        };
        
        Audio & operator|(Audio & audio2){
            numberOfSamples = numberOfSamples+audio2.numberOfSamples;
            for (typename std::vector<std::pair<T,T>>::iterator i=audio2.data_buffer.begin(); i!=audio2.data_buffer.end(); i++){
                data_buffer.push_back(*i);
            }
            return *this;
        }
        
        Audio & operator*(std::pair<float,float> vf){
            if (channel == 2){
                for (typename std::vector<std::pair<T,T>>::iterator i=data_buffer.begin(); i!=data_buffer.end(); i++){
                    i->first = i->first * vf.first;
                    i->second = i->second * vf.second;
                }
            }
        
        return *this;
        }
        
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
        
        void add(Audio & audio2){
            *this = *this + audio2;
        }
        
        void cut(int r1, int r2){
            std::pair<int,int> range(r1,r2);
            *this = *this^range;
        }
        
        void rangedAdd(Audio & audio2, int r1, int r2, int s1, int s2){
            if (r1-r2 == s1-s2){
                r1 = r1*sampleRate;
                r2 = r2*sampleRate;
                s1 = s1*sampleRate;
                s2 = s2*sampleRate;
                std::cout << r1 << ":" << r2 << ":" << s1 << ":" << s2 << std::endl;
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
        
        void concatenate(Audio & audio2){
            *this =  *this | audio2;
        }
        
        void volume(float f1, float f2){
            std::pair<float,float> factor(f1,f2);
            *this = *this * factor;
        }
        
        void reverse(){
            std::reverse(data_buffer.begin(), data_buffer.end());
        }
        
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
            std::cout << "The RMS value of the left channel of audio file is: ";
            std::cout << left_rms;
            std::cout << " " << std::endl;
            std::cout << "The RMS value of the right channel of audio file is: ";
            std::cout << right_rms;
            std::cout << " " << std::endl;
        }
        
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
        }
            
    };
    
    
}


#endif /* AUDIO_H */

