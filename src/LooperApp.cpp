#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CinderImGui.h"

#include "Looper.h"
#include "cinder/Breakpoint.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LooperApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void mouseMove(MouseEvent event) override;
	void update() override;
	void draw() override;
	void keyUp(KeyEvent event) override;
	void cleanup() override;
	LooperRef loop;
};

void LooperApp::setup()
{

	Looper testLoop;

	loop = make_shared<Looper>();

	ui::initialize();
}

void LooperApp::mouseDown(MouseEvent event)
{
}

void LooperApp::update()
{


	if (loop)
	{
		ui::Begin();
		loop->pushData(getMousePos().y - 200);
		(loop->isRecording()) ? ui::LabelText("Status:", "RECORDING") : ui::LabelText("Status:","Playing");
		ui::Separator();
		ui::Value("Index", loop->getIndex());
		ui::Value("Buffer size", loop->getBufferSize());
		
		

		float position = float((float)loop->getIndex() / (float)loop->getBufferSize());
		ui::Value("pos", position);
		ui::SliderFloat("position", &position, 0.0, 1.0);
		static bool smooth, smoothCos = false;


		if (loop->getBufferSize() > 0){
			ui::PlotLines("function", loop->getDataRef(), loop->getBufferSize(), 0, "", 0, 769, ImVec2(200, 200));	
		}
		if (ui::CollapsingHeader("Options")){
			if (ui::Button("smooth linear")){
				loop->smoothLin();
			}
			if (ui::Button("smooth cosine")){
				loop->smoothCos();
			}
		}
		if (ui::Button("inverse"))
		{
			loop->inverse();
		}
		ui::End();
	}
	
}

void LooperApp::draw()
{
	gl::clear(Color(1, 1, 1));


	gl::GlslProgRef shader = gl::getStockShader(gl::ShaderDef().color());
	auto g = geom::Circle().center(vec2(getWindow()->getCenter())).radius(10);
	TriMesh::Format geomFmt = TriMesh::Format().positions().colors();
	TriMesh mesh(g, geomFmt);
	gl::BatchRef batch = gl::Batch::create(mesh, shader);
	static int offset = 0;
	static bool offsetTest = true;
	static bool mixTest = false;

	

	ui::Checkbox("offset test", &offsetTest);
	ui::Checkbox("mixing test", &mixTest);
	
	if (ui::Button("TEST WINDOW")){ ui::ShowTestWindow(); }


	if (offsetTest)
	{
		ui::SliderInt("offset", &offset, -100, 100);

		gl::pushModelMatrix();
		gl::translate(vec2(0, loop->getValue()));
		batch->draw();
		gl::popModelMatrix();


		for (size_t i = 0; i < 10; i++)
		{
			gl::ScopedModelMatrix scpModelMtx;
			gl::translate(vec2(i * 50, loop->getValueWithOffset(i*offset)));
			gl::color(Color(CM_HSV, i / (2 * M_PI), 1, 1));
			if (loop)ui::Value(toString(i*offset).c_str(), loop->getValueWithOffset(i*offset));

			batch->draw();
		}
	}
	if (mixTest){
		static float dryWet = 0;
		ui::SliderFloat("dry/wet", &dryWet, 0, 1);
		gl::pushModelMatrix();
		gl::translate(vec2(0, loop->getValueMixed(dryWet)));
		batch->draw();
		gl::popModelMatrix();

	}
	ui::Render();
}

void LooperApp::keyUp(KeyEvent event)
{
	if (event.getCode() == KeyEvent::KEY_SPACE)
	{
		loop->toggleRecord();
	}
	if (event.getCode() == KeyEvent::KEY_RETURN)
	{
		loop->clear();
	}

}

void LooperApp::cleanup()
{
	ui::Shutdown();
}


CINDER_APP(LooperApp, RendererGl)

void LooperApp::mouseMove(MouseEvent event)
{
}
