#include <vector>
#include "cinder/app/App.h"
#include "CinderImGui.h"
#include "cinder/Rand.h"

class Looper;

typedef std::shared_ptr<Looper> LooperRef;


class Looper
{
public:
	Looper();
	~Looper();

	void toggleRecord();

	float getValue();
	float getValueWithOffset(const int &offset);
	// mixing (dry/wet implementation)
	float getValueMixed(float mix);
	
	int getBufferSize(){ return data.size(); }
	int getIndex() { return index; }
	void pushData(const float& value);
	void clear();



	// highly experimental thing for testing purposes
	const float* getDataRef(){return &data[0]; }

	bool isRecording(){ return recording; }
	void smoothLin();
	void smoothCos();

private:
	float passThrough;
	std::vector<float> data;
	void update();
	int index;

	bool recording;
	inline double LinearInterpolate(float y1, float y2,	float mu);
	inline double CosineInterpolate(double y1, double y2,double mu);
};
