#pragma once
#include<string>
#include<vector>
#include<cmath>

#define GAME_MODULE_BEGIN namespace RurDog {
#define GAME_MODULE_END }

GAME_MODULE_BEGIN

#define SWAP(x,y) auto temp = x; x = y; y = temp;


/// <summary>
/// get the maximum one in the given list by use greater-than sign
/// </summary>
/// <typeparam name="T">the type of given list</typeparam>
/// <param name="list">initializer list of elements you want to compare</param>
/// <returns>the maximum element</returns>
template<typename T>
T Max(std::initializer_list<T> list)
{
	if (list.size() == 0) return 0;
	auto it = list.begin();
	auto max = *it;
	it++;
	for (; it != list.end(); it++)
	{
		if (*it > max)
			max = *it;
	}
	return max;
}
/// <summary>
/// get the minimum one in the given list by use greater-than sign
/// </summary>
/// <typeparam name="T">the type of given list</typeparam>
/// <param name="list">initializer list of elements you want to compare</param>
/// <returns>the minimum element</returns>
template<typename T>
T Min(const std::initializer_list<T> list)
{
	auto it = list.begin();
	auto min = *it;
	it++;
	for (; it != list.end(); it++)
	{
		if (*it < min)
			min = *it;
	}
	return min;
}




#define DEBUG_MARK "[DEBUG]: "
#ifdef DEBUG_MODE
#define TIME_STAMP "\t --%s "
inline void Debug(const char* s)
{
	std::string str(DEBUG_MARK);
	str.append(s);
	str.append(TIME_STAMP);
	auto t = time(NULL);
	SDL_Log(str.c_str(), asctime(localtime(&t)));
}
#else 
inline void Debud(const char* s)
{
	return;
}
#endif
GAME_MODULE_END
