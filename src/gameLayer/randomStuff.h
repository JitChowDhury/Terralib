#pragma once
#include <random>

//returns a float in (min,max)
float getRandomFloat(std::ranlux24_base& rng, float min, float max);

//returns an int in (min,max)
int getRandomInt(std::ranlux24_base& rng, int min, int max);

bool getRandomChance(std::ranlux24_base& rng, float chance);