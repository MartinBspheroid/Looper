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
		ui::Checkbox("Recording", &loop->recording);
		ui::Checkbox("Playing", &loop->plaing);
		ui::Separator();
		ui::Value("Index", loop->getIndex());
		ui::Value("Buffer size", loop->getBufferSize());
		if (loop->getIndex() > 50 && loop->getBufferSize() > 100){
			//	CI_BREAKPOINT();
		}

		float position = float((float)loop->getIndex() / (float)loop->getBufferSize());
		ui::Value("pos", position);
		ui::SliderFloat("position", &position, 0.0, 1.0);
		static bool smooth = false;
		if (ui::Button("smooth")){
			loop->smooth();
		}

		if (loop->getBufferSize() > 0){
			ui::PlotLines("function", loop->getDataRef(), loop->getBufferSize(), 0, "", 0, 769, ImVec2(400, 400));
			
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
if(loop)	ui::SliderInt("offset", &offset, -100, 100);

	gl::pushModelMatrix();
	gl::translate(vec2(0, loop->getValue()));
	batch->draw();
	gl::popModelMatrix();


	for (size_t i = 0; i < 10; i++)
	{
		gl::ScopedModelMatrix scpModelMtx;
		gl::translate(vec2(i * 50, loop->getDataWithOffset(i*offset)));
		gl::color(Color(CM_HSV, i / (2 * M_PI), 1, 1));
		if(loop)ui::Value(toString(i*offset).c_str(), loop->getDataWithOffset(i*offset));

		batch->draw();
	}



	ui::Render();
}

void LooperApp::keyUp(KeyEvent event)
{
	if (event.getCode() == KeyEvent::KEY_SPACE)
	{
		loop->record();
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
