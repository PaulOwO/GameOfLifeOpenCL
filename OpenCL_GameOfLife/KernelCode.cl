

int getLinear(int x, int y)
{
	return x + y * 20;
}

uint CountNeighbor(char* array, int x, int y)
{
	size_t neighbourCount = 0;
	for (auto w = -1; w < 2; w++)
	{
		for (auto z = -1; z < 2; z++)
		{
			if (w + x < 0)
			{
				continue;
			}
			if (z + y < 0)
			{
				continue;
			}
			if (w + x >= 20)
			{
				continue;
			}
			if (z + y >= 20)
			{
				continue;
			}
			if (w == 0 && z == 0)
			{
				continue;
			}

			if (array[getLinear(w + x, z + y)] == '1')
			{
				neighbourCount++;
			}
		}
	}
	return neighbourCount;
}

__kernel void simple(
	__global read_only char* in1,
	__global write_only char* out)
{
	const uint pos = get_global_id(0);
	uint x = pos % 20;
	uint y = pos / 20;

		size_t neighbourCount = 0;
		neighbourCount = CountNeighbor(in1, x, y);

		if (in1[getLinear(x, y)] == '0')
		{
			if (neighbourCount == 3)
			{
				out[getLinear(x, y)] = '1';
			}
			else
			{
				out[getLinear(x, y)] = '0';
			}
		}
		else if (in1[getLinear(x, y)] == '1')
		{
			if (neighbourCount == 2 || neighbourCount == 3)
			{
				out[getLinear(x, y)] = '1';
			}
			else
			{
				out[getLinear(x, y)] = '0';
			}
		}
}
	