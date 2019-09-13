#pragma once
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

namespace PerlinNoise{

    class Perlin3D{

        private:

            //sets up std::shuffle
            std::random_device rng;

            //hash
            int p[512];
            //current seed value
            double seed_v;

            //populates the hash array using the seed passed
            void pop_p(double seed){
                //PRNG passed to std::shuffle
                std::mt19937 urng(seed);
                for(int i=0; i<256; i++){
                    p[i] = i;
                }
                srand(seed);
                std::shuffle(std::begin(p), std::begin(p)+256, urng);
                for(int i=0; i<256; i++){
                    p[256+i] = p[i];
                }
            }

            //fade curve function
            static double const fade(double t){
                return t*t*t*(t*(t*6 - 15) + 10);
            }

            //linear interpolation function
            static double const lerp(double t, double a, double b){
                return a + t*(b-a);
            }

            //determines the pseudorandom gradient
            static double const grad(int hash, double x, double y, double z){
                int h = hash & 15;
                double u = h<8 ? x : y;
                double v = h<4 ? y : h==12||h==14 ? x : z;
                return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
            }

        public:

            //returns the value of the current seed
            double currentSeed(){
                return seed_v;
            }

            //sets seed to value passed or random value
            void seed(double newSeed=NULL){
                newSeed!=NULL ? seed_v = newSeed : seed_v = (double)rng();
                pop_p(newSeed);
            }

            //generates regular 3D noise
            double const noise(double x, double y, double z){

                //find cube that contains point
                int xint = (int)floor(x) & 255;
                int yint = (int)floor(y) & 255;
                int zint = (int)floor(z) & 255;
        
                //find point within that cube
                x -= floor(x);
                y -= floor(y);
                z -= floor(z);

                //find fade curve for each
                double u = fade(x);
                double v = fade(y);
                double w = fade(z);

                //hash coordinates of 8 cube corners
                int A = p[xint]+yint, B = p[xint+1]+yint;
                int AA = p[A]+zint,   BA = p[B]+zint;
                int AB = p[A+1]+zint, BB = p[B+1]+zint;

                //mix results from 8 corners
                //using linear interpolation
                double aa1 = grad(p[AA], x, y, z);
                double ba1 = grad(p[BA], x-1, y, z);
                double ab1 = grad(p[AB], x, y-1, z);
                double bb1 = grad(p[BB], x-1, y-1, z);
                double aa2 = grad(p[AA+1], x, y, z-1);
                double ba2 = grad(p[BA+1], x-1, y, z-1);
                double ab2 = grad(p[AB+1], x, y-1, z-1);
                double bb2 = grad(p[BB+1], x-1, y-1, z-1);
                double lerpU1 = lerp(u, aa1, ba1);
                double lerpU2 = lerp(u, ab1, bb1);
                double lerpU3 = lerp(u, aa2, ba2);
                double lerpU4 = lerp(u, ab2, bb2);
                double lerpV1 = lerp(v, lerpU1, lerpU2);
                double lerpV2 = lerp(v, lerpU3, lerpU4);
                return lerp(w, lerpV1, lerpV2);
            }
            
            //generates 3D noise using octaves for finer detail
            double const octaveNoise(double x, double y, double z, int octaves=6, double impedence=2){
                
                double total = 0.0;
                double amplitude = 1.0;

                for(int i=0; i<octaves; i++){
                    total += noise(x, y, z) * amplitude;
                    x *= impedence;
                    y *= impedence;
                    z *= impedence;
                    amplitude *= 1/impedence;
                }
                return total;
            }
    };

    class Perlin2D : private Perlin3D{
        public:
            
            //setting up std::shuffle
            std::random_device rng;

            //current seed value
            double seed_v;

            //returns the value of the current seed
            double currentSeed(){
                return seed_v;
            }

            //sets seed to the vaule passed or random value
            void seed(double newSeed=NULL){
                newSeed!=NULL ? seed_v = newSeed : seed_v = (double)rng();
                Perlin3D::seed(seed_v);
            }

            //reuses the 3D noise function
            double const noise(double x, double y){
                return Perlin3D::noise(x, y, 0.0);
            }
            
            //reuses the 3D octaveNoise function
            double const octaveNoise(double x, double y, int octaves=6, int impedence=2){
                return Perlin3D::octaveNoise(x, y, 0.0, octaves, impedence);
            }
            
            //sets the seed for 2D and 3D noise
            //can be changed individually later
            Perlin2D(){
                seed(rng());
            }
    };
}