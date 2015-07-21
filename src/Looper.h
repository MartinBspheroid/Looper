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

	void record(){
		recording = !recording;
	}

	bool isRecording(){ return recording; }
	bool isPlaying() { return plaing; }
	int getBufferSize(){ return data.size(); }
	int testCounter, mouseCounter;
	bool recording, plaing;
	float getValue(){
		if (data.size() > 0 && !recording)
		{
			return data.at(index);
		}
		else {
			return passThrough;
		};
	}
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



	const int testFunc(){

		for (size_t i = 0; i < 100; i++)
		{
			data.push_back(ci::randFloat(-1, 1));
		}
		
		smooth();
		data.clear();
		for (size_t i = 0; i < 100; i++)
		{
			data.push_back(ci::randFloat(-100, 100));
		}
		smooth();
		data.clear();
		return 1;

	}
	void smooth(){


		/*auto m = minmax_element(data.begin(), data.end());
		float scale = m.second - m.first;

		vector<ci::vec2> points;

		for (size_t i = 0; i < data.size(); i++)
		{
			points.emplace_back(i, data.at(i));
		}
		
		ci::BSpline2f spline = ci::BSpline2f(points, 1, false, false);
		
		float total = spline.getLength(0, 1);
		ci::app::console() << "total > " << total << endl;
		float inc = total / (float)data.size();
		ci::app::console() << "inc> " << inc<< endl;
		std::vector<float> newData;
		for (float i = 0; i <  total; i+= inc){
			newData.push_back(spline.getPosition(i*total).y);
		}
		data = newData;
		*/

		for (size_t i = 0; i < data.size()-1; i++)
		{
			data.at(i) = LinearInterpolate(data.at(i), data.at(i + 1), 0.5);
		}

		data.back() = LinearInterpolate(data.back(), data.front(), 0.5);
	}

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

	const float* getDataRef(){
		return &data[0];
	}

	int getIndex() { return index; }
	float getDataWithOffset(const int &offset){
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

};

Looper::Looper()
{
	recording = plaing = false;
	
	ci::app::getWindow()->getSignalPostDraw().connect([this]() {update(); });
	index = 0;
	data.reserve(0);
	testCounter = mouseCounter = 0;
	passThrough = 0;

	
}

Looper::~Looper()
{
}
typedef std::shared_ptr<Looper> LooperRef;