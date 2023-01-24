#pragma once
#include <random>

class Random
{
public:
	Random()
		:generator(0)
	{
		
	}
	
	float getRandomInt(int a , int b)
	{
		std::uniform_int_distribution<int> uint_dist{ a,b };
		return uint_dist(generator);
	}
	float getRandomInt(int b = 0)
	{
		std::uniform_int_distribution<int> uint_dist{ 0,b };
		return uint_dist(generator);
	}
private:

	std::random_device rd;
	std::mt19937 generator;
};