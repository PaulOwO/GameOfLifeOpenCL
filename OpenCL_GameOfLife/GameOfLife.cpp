
#include <array>
#include <iostream>
#include <Windows.h>

using array = std::array<std::array<char, 20>, 20>;

size_t CountNeighbor(const array& array, int x, int y)
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

			if (array[w + x][z + y] == '1')
			{
				neighbourCount++;
			}
		}
	}
	return neighbourCount;
}

array compute(const array& state)
{
	array newState{};
	for (auto x = 0; x < 20; x++)
	{
		for (auto y = 0; y < 20; y++)
		{
			size_t neighbourCount = 0;
			neighbourCount = CountNeighbor(state, x, y);
			
			if (state[x][y] == '0')
			{
				if (neighbourCount == 3)
				{
					newState[x][y] = '1';
				}
				else
				{
					newState[x][y] = '0';
				}
			}
			else if (state[x][y] == '1')
			{
				if (neighbourCount == 2 || neighbourCount == 3)
				{
					newState[x][y] = '1';
				}
				else
				{
					newState[x][y] = '0';
				}
			}
		}
	}
	return newState;
}

std::ostream& operator << (std::ostream& os, const array& arr)
{
	for (auto x = 0; x < 20; x++)
	{
		for (auto y = 0; y < 20; y++)
		{
			if( arr[x][y] == '1')
			{
				os << " @ ";
			}
			else
			{
				os << " . ";
			}
		}
		os << std::endl ;
	}
	return os;
}

//int main()
//{
//	array initialState;
//	for (auto x = 0; x < 20; x++)
//	{
//		for (auto y = 0; y < 20; y++)
//		{
//			initialState[x][y] = static_cast<char>(rand() % 2 + 48);
//		}
//	}
//	std::cout << initialState;
//	Sleep(25);
//	std::cout << "\033[H";
//	for (auto i = 0; i < 1000; i++)
//	{
//		auto newState = compute(initialState);
//		std::cout << newState;
//		initialState = newState;
//		Sleep(500);
//		std::cout << "\033[H";
//	}
//	return 0;
//}