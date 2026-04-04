#include "worldGenerator.h"
#include "randomStuff.h"
#include <FastNoiseSIMD.h>

void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 2000;
	const int h = 500;

	gameMap.create(w, h);
	std::ranlux24_base rng(seed++);

	std::unique_ptr<FastNoiseSIMD> dirtNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	//std::unique_ptr<FastNoiseSIMD> stoneNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());
	std::unique_ptr<FastNoiseSIMD> cavesNoiseGenerator(FastNoiseSIMD::NewFastNoiseSIMD());

	dirtNoiseGenerator->SetSeed(seed++);
	//stoneNoiseGenerator->SetSeed(seed++);
	cavesNoiseGenerator->SetSeed(seed++);

	dirtNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	dirtNoiseGenerator->SetFractalOctaves(6);
	dirtNoiseGenerator->SetFractalGain(0.4f);
	dirtNoiseGenerator->SetFrequency(0.005f);

	//stoneNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	//stoneNoiseGenerator->SetFractalOctaves(4);
	//stoneNoiseGenerator->SetFrequency(0.01);

	cavesNoiseGenerator->SetNoiseType(FastNoiseSIMD::NoiseType::SimplexFractal);
	cavesNoiseGenerator->SetFractalOctaves(3);
	cavesNoiseGenerator->SetFrequency(0.006f);

	float* dirtNoise = FastNoiseSIMD::GetEmptySet(w);
	//float* stoneNoise = FastNoiseSIMD::GetEmptySet(w);
	float* cavesNoise = FastNoiseSIMD::GetEmptySet(w * h);


	dirtNoiseGenerator->FillNoiseSet(dirtNoise, 0, 0, 0, w, 1, 1);
	//stoneNoiseGenerator->FillNoiseSet(stoneNoise, 0, 0, 0, w, 1, 1);
	cavesNoiseGenerator->FillNoiseSet(cavesNoise, 0, 0, 0, h, w, 1);

	for (int i = 0; i < w; i++)
	{
		dirtNoise[i] = (dirtNoise[i] + 1) / 2;
		//stoneNoise[i] = (stoneNoise[i] + 1) / 2;

		//stoneNoise[i] = std::pow(stoneNoise[i], 2);
	}
	for (int i = 0; i < w*h; i++)
	{
		cavesNoise[i] = (cavesNoise[i] + 1) / 2;
	}

	auto getCaveNoise = [&](int x, int y)
		{
			return cavesNoise[x + y * w];
		};

	int dirtOffsetStart = -5;
	int dirtOffsetEnd= 35;

	int stoneHeightStart = 80;
	int stoneHeightEnd = 170;

	int keepDirectionTimeStone = getRandomInt(rng, 5, 40);
	int directionStone = getRandomInt(rng, -2, 2);

	for (int x = 0; x < w; x++)
	{
		int stoneHeight = stoneHeightStart +
			(stoneHeightEnd - stoneHeightStart) * getCaveNoise(x, 0);
		int dirtHeight = dirtOffsetStart + (dirtOffsetEnd - dirtOffsetStart) * dirtNoise[x];

		dirtHeight = stoneHeight - dirtHeight;

		for (int y = 0; y < h; y++)
		{
			Block b;
			if (y > dirtHeight)
			{
				b.type = Block::dirt;
			}

			if (y == dirtHeight)
			{
				b.type = Block::grassBlock;
			}
			if (y >= stoneHeight)
			{
				b.type = Block::stone;
			}

			if (getCaveNoise(x, y) < 0.30)
			{
				b.type = Block::air;
			}
			gameMap.getBlockUnsafe(x, y) = b;
		}
	}

	FastNoiseSIMD::FreeNoiseSet(dirtNoise);
	//FastNoiseSIMD::FreeNoiseSet(stoneNoise);
	FastNoiseSIMD::FreeNoiseSet(cavesNoise);

	#pragma region perlin worms
	
	for (int i = 0; i < 20; i++)
	{
		//pick a random starting point
		//it's important for x and y to be floats
		float x = getRandomInt(rng, 10, w - 10);
		float y = getRandomInt(rng, 51, h - 10);

		//initial movement direction(-1 to 1 range)
		float dirX = (getRandomFloat(rng, -1, 1));
		float dirY = (getRandomFloat(rng, -1, 1));

		int wormLength = getRandomInt(rng, 200, 700);
		float radius = 2.5f;
		int changeDirectionTime = getRandomInt(rng, 5, 20);

		for (int j = 0; j < wormLength; j++)
		{
			int intRadius = std::ceil(radius);
			for (int  ox = -intRadius; ox <= intRadius; ox++)
			{
				for (int oy = -intRadius; oy <= intRadius; oy++)
				{
					float distSq = ox * ox + oy * oy;
					if (distSq <= radius * radius)
					{
						int digX = x + ox;
						int digY = y + oy;

						auto b = gameMap.getBlockSafe(digX, digY);
						if (b)
						{
							b->type = Block::air;
						}
					}
				}
			}
			changeDirectionTime--;
			if (changeDirectionTime <= 0)
			{
				changeDirectionTime = getRandomInt(rng, 5, 20);
				if(getRandomChance(rng,0.7))
				{
					float keepFactor = 0.8;

					dirX = dirX * keepFactor + (getRandomFloat(rng, -1, 1)) * (1.f - keepFactor);
					dirY = dirY * keepFactor + (getRandomFloat(rng, -1, 1)) * (1.f - keepFactor);
				}
				else
				{
					float keepFactor = 0.2;

					dirX = dirX * keepFactor + (getRandomFloat(rng, -1, 1)) * (1.f - keepFactor);
					dirY = dirY * keepFactor + (getRandomFloat(rng, -1, 1)) * (1.f - keepFactor);
	
				}


			}

			x += dirX * 1.5f;
			y += dirY * 1.5f;

			radius += (getRandomFloat(rng, -0.2, 0.2));
			radius = std::clamp(radius, 2.2f,8.5f);
		}


	}
}
