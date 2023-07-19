#pragma once

#include "Types.h"

template<typename T>
class WeightedBag
{
	private:
	//boost::random_device mRng;
	int mCount;
	std::vector<int> mWeights;
	std::vector<T> mValues;
	void RemoveValue(int index);

	public:
	WeightedBag();
	int AddValue(T value, int weight = 1);
	T GetRandomValue(bool remove = false);
	bool IsEmpty();
	void Print();
};

template<typename T>
inline void WeightedBag<T>::RemoveValue(int index)
{
	auto itW = mWeights.begin();
	auto itV = mValues.begin();

	//std::cout << "\tWeightedBag removing index " << index << " / " << mWeights.size()-1 << std::endl;

	std::advance(itW, index);
	std::advance(itV, index);

	mValues.erase(itV);

	if (mWeights.size() == 1)
	{
		mWeights.erase(itW);
		mCount = 0;
		return;
	}

	if (index == mWeights.size() - 1)
	{
		//std::cout << "Pulled last element" << std::endl;
		mCount -= ((*itW) - mWeights[index - 1]);
		mWeights.erase(itW);
		return;
	}
	else if (index == 0)
	{
		//std::cout << "Pulled first element" << std::endl;
		int d = (*itW);
		mWeights.erase(itW);
		for (int i = 0; i < mWeights.size(); ++i)
		{
			mWeights[i] = mWeights[i] - d;
		}
		mCount -= d;
		return;
	}

	int d = mWeights[index + 1] - mWeights[index];
	for (int i = index; i < mWeights.size(); ++i)
	{
		mWeights[i] = mWeights[i] - d;
	}
	mWeights.erase(itW);
	mCount -= d;
}

template<typename T>
inline WeightedBag<T>::WeightedBag()
{
	mCount = 0;
}

template<typename T>
inline int WeightedBag<T>::AddValue(T value, int weight)
{
	mCount += weight;
	mValues.push_back(value);
	mWeights.push_back(mCount);
	return mCount;
}

template<typename T>
inline T WeightedBag<T>::GetRandomValue(bool remove)
{
	int n = rand() % mCount;

	int i = 0;

	//std::cout << "\t>> WeightedBag rolled " << n << " / " << mCount << " == ";

	for (auto it = mWeights.begin(); it != mWeights.end(); ++it)
	{
		if ((*it) > n) { break; }
		++i;
	}

	T value = mValues[i];

	if (remove)
	{
		RemoveValue(i);
	}

	return value;
}

template<typename T>
inline bool WeightedBag<T>::IsEmpty()
{
	return (mValues.size() == 0);
}

template<typename T>
inline void WeightedBag<T>::Print()
{
	for (int i = 0; i < mValues.size(); ++i)
	{
		std::cout << mValues[i] << " : " << mWeights[i] << std::endl;
	}
}
