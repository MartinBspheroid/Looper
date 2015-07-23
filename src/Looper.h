#include <vector>
#include "CinderImGui.h"
#include "cinder/Filter.h"
#include "cinder/BSpline.h"
#include "cinder/Rand.h"
using namespace std;

class Looper
{
public:
	Looper();
	~Looper();

	void toggleRecord(){
		recording = !recording;
	}

	float getValue(){
		if (data.size() > 0 && !recording)
		{
			return data.at(index);
		}
		else {
			return passThrough;
		};
	}
	float getValueWithOffset(const int &offset){
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
	// mixing (dry/wet implementation)
	float getValueMixed(float mix){
		return (getValue() * mix) + (passThrough * (1-mix));
	}
	
	int getBufferSize(){ return data.size(); }
	int getIndex() { return index; }
	void pushData(const float& value){
		passThrough = value;
		if (recording)
		{
			data.push_back(value);

			//index++;
		}
	}
	void clear(){


		data.clear();
		index = 0;
	}

	// TODO this does not work
	void invert(){
		auto maxValue = max_element(data.begin(), data.end());
		for (auto &v : data){
			v = (maxValue - data.begin()) - v;
		}
	}


	// highly experimental thing for testing purposes
	const float* getDataRef(){
		return &data[0];
	}

	const int testFunc(){

		for (size_t i = 0; i < 100; i++)
		{
			data.push_back(ci::randFloat(-1, 1));
		}
		
		smoothLin();
		data.clear();
		for (size_t i = 0; i < 100; i++)
		{
			data.push_back(ci::randFloat(-100, 100));
		}
		smoothLin();
		data.clear();
		return 1;

	}
	
	bool isRecording(){ return recording; }
	void smoothLin(){

		for (size_t i = 0; i < data.size()-1; i++)
		{
			data.at(i) = LinearInterpolate(data.at(i), data.at(i + 1), 0.5);
		}

		data.back() = LinearInterpolate(data.back(), data.front(), 0.5);
	}
	void smoothCos(){

		for (size_t i = 0; i < data.size() - 1; i++)
		{
			data.at(i) = CosineInterpolate(data.at(i), data.at(i + 1), 0.5);
		}

		data.back() = CosineInterpolate(data.back(), data.front(), 0.5);
	}

private:
	float passThrough;
	vector<float> data;
	void update(){
		if (data.size() > 0)
		{
			index++;
			index %= data.size();
		}

	}
	int index;

	bool recording;
	float LinearInterpolate(
		float y1, float y2,
		float mu)
	{
		return(y1*(1 - mu) + y2*mu);
	}
	double CosineInterpolate(
		double y1, double y2,
		double mu)
	{
		double mu2;

		mu2 = (1 - cos(mu*3.1415)) / 2;
		return(y1*(1 - mu2) + y2*mu2);
	}
};

Looper::Looper()
{
	recording = false;
	
	ci::app::getWindow()->getSignalPostDraw().connect([this]() {update(); });
	index = 0;
	passThrough = 0;

	
}

Looper::~Looper()
{
}
typedef std::shared_ptr<Looper> LooperRef;