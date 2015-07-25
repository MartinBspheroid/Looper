#include "Looper.h"

using namespace std;
using namespace ci;
using namespace ci::app;

void Looper::toggleRecord()
{
	
	recording = !recording;
	index--; /// ugly hack but it works
//	console() << "Recording going" << recording << endl;
}

float Looper::getValue()
{
	if (data.size() > 0 && !recording)
	{
 		return data.at(index);
	}
	else {
		return passThrough;
	};
}

float Looper::getValueWithOffset(const int &offset)
{
	if (data.size() == 0){
		return passThrough;
	}

	if ((index + offset) >= data.size()){ /// handle out of bounds access with wrapping around buffer
		return data.at((index + offset) % data.size());
	}
	if ((index - offset) < 0){
		return data.at(data.size() - (offset - index)); // handle out of bounds access with negative index access
	}

	return data.at(index + offset); // return value with offset index
}

float Looper::getValueMixed(float mix)
{
	return (getValue() * mix) + (passThrough * (1 - mix));
}

void Looper::pushData(const float& value)
{
	passThrough = value;
	if (recording && index >= data.size())
	{
		data.push_back(value);
		console() << "pushed value to index " << index << endl;

		//index++;
	}
	else if (recording && index < data.size())
	{
		data.at(index) = value;
		console() << "overdub value to index " << index << endl;
	}
}

void Looper::clear()
{
	data.clear();
	index = 0;
}

void Looper::smoothLin()
{
	for (size_t i = 0; i < data.size() - 1; i++)
	{
		data.at(i) = LinearInterpolate(data.at(i), data.at(i + 1), 0.5);
	}

	data.back() = LinearInterpolate(data.back(), data.front(), 0.5);
}

void Looper::smoothCos()
{
	for (size_t i = 0; i < data.size() - 1; i++)
	{
		data.at(i) = CosineInterpolate(data.at(i), data.at(i + 1), 0.5);
	}

	data.back() = CosineInterpolate(data.back(), data.front(), 0.5);
}

void Looper::update()
{
	if (data.size() > 0)
	{
		index++;
		//console() << "update index " << index << endl;
	}
	if (!recording && data.size() > 0){
		index %= data.size();
		console() << "rollBack" << endl;
	}
}

inline double Looper::LinearInterpolate(float y1, float y2, float mu)
{
	return(y1*(1 - mu) + y2*mu);
}

double Looper::CosineInterpolate(double y1, double y2, double mu)
{
	double mu2;

	mu2 = (1 - cos(mu*3.1415)) / 2;
	return(y1*(1 - mu2) + y2*mu2);
}

Looper::Looper()
{
	recording = false;

	ci::app::getWindow()->getSignalPostDraw().connect([this]() {update(); });
	index = 0;
	passThrough = 0;
}

Looper::~Looper(){}

void Looper::inverse()
{
	reverse(data.begin(), data.end());
}
