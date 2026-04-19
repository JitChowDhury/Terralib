#include "worldGenerator.h"
#include "randomStuff.h"
#include <FastNoiseSIMD.h>
#include <structure.h>
#include <saveMap.h>

void generateWorld(GameMap& gameMap, int seed)
{
	const int w = 2000;
	const int h = 500;

	gameMap.create(w, h);
	std::ranlux24_base rng(seed++);

	Structure treeStructure;
	loadBlockDataFromFile(treeStructure.mapData, treeStructure.w, treeStructure.h, RESOURCES_PATH "structures/tree.bin");

	Structure houseStructure;
	loadBlockDataFromFile(houseStructure.mapData, houseStructure.w, houseStructure.h, RESOURCES_PATH "structures/house.bin");

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
		// Random starting point
		float x = (float)getRandomInt(rng, 10, w - 10);
		float y = (float)getRandomInt(rng, 51, h - 10);

		// Initial direction
		float dirX = getRandomFloat(rng, -1, 1);
		float dirY = getRandomFloat(rng, -1, 1);

		// Normalize initial direction
		float len = sqrt(dirX * dirX + dirY * dirY);
		if (len != 0)
		{
			dirX /= len;
			dirY /= len;
		}

		int wormLength = getRandomInt(rng, 200, 700);

		float radius = getRandomFloat(rng, 2.5f, 4.5f);
		float speed = getRandomFloat(rng, 1.2f, 2.0f);

		// Controls how curvy the worm is (personality)
		float turnStrength = getRandomFloat(rng, 0.05f, 0.25f);

		for (int j = 0; j < wormLength; j++)
		{
			int intRadius = (int)std::ceil(radius);

			// Dig circle
			for (int ox = -intRadius; ox <= intRadius; ox++)
			{
				for (int oy = -intRadius; oy <= intRadius; oy++)
				{
					float distSq = ox * ox + oy * oy;
					if (distSq <= radius * radius)
					{
						int digX = (int)x + ox;
						int digY = (int)y + oy;

						auto b = gameMap.getBlockSafe(digX, digY);
						if (b)
						{
							b->type = Block::air;
						}
					}
				}
			}

			// Smooth turning (no jitter)
			dirX += getRandomFloat(rng, -1.0f, 1.0f) * turnStrength;
			dirY += getRandomFloat(rng, -1.0f, 1.0f) * turnStrength;

			// Slight downward bias (natural caves)
			dirY += 0.02f;

			// Normalize direction
			float len = sqrt(dirX * dirX + dirY * dirY);
			if (len != 0)
			{
				dirX /= len;
				dirY /= len;
			}

			// Move
			x += dirX * speed;
			y += dirY * speed;

			// Smooth radius change
			float targetRadius = radius + getRandomFloat(rng, -0.3f, 0.3f);
			radius = radius * 0.9f + targetRadius * 0.1f;

			radius = std::clamp(radius, 2.5f, 7.5f);
		}
	}

	#pragma endregion


	#pragma region fill trees

	for (int x = 0; x < w; x++)
	{
		if (getRandomChance(rng, 0.04))
		{
			for (int y = 0; y < h; y++)
			{
				auto type = gameMap.getBlockUnsafe(x, y).type;

				if (type == Block::air)
				{
					continue;
				}

				if (type == Block::grassBlock)
				{
					// plant tree
					Vector2 spawnPos{ (float)x, (float)y };

					spawnPos.x -= treeStructure.w / 2;
					spawnPos.y -= treeStructure.h;

					treeStructure.pasteIntoMap(gameMap, spawnPos);

					x += 3; // we don't plant a tree to overlap this one
					break;
				}
				else
				{
					break;
				}
			}
		}
	}

	#pragma endregion

	#pragma region fill houses

	for (int x = 0; x < w; x++)
	{
		if (getRandomChance(rng, 0.04))
		{
			for (int y = 0; y < h; y++)
			{
				auto type = gameMap.getBlockUnsafe(x, y).type;

				if (type == Block::air) continue;

				if (type == Block::grassBlock)
				{
					Vector2 spawnPos{ (float)x, (float)y };

					spawnPos.x -= houseStructure.w / 2;
					spawnPos.y -= houseStructure.h - 1;

					
					bool valid = true;
					int baseY = y;

					for (int i = 0; i < houseStructure.w; i++)
					{
						int checkX = (int)spawnPos.x + i;

						int foundY = -1;
						for (int yy = 0; yy < h; yy++)
						{
							auto t = gameMap.getBlockUnsafe(checkX, yy).type;
							if (t != Block::air)
							{
								foundY = yy;
								break;
							}
						}

						if (foundY == -1 || abs(foundY - baseY) > 2)
						{
							valid = false;
							break;
						}
					}

					if (valid)
					{
						houseStructure.pasteIntoMap(gameMap, spawnPos);
						x += houseStructure.w;
					}

					break;
				}
				else break;
			}
		}
	}

	#pragma endregion
}
