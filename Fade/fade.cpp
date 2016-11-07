//模型时隐时现的效果
#include <osg/Material>

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgAnimation/EaseMotion>

#include <osg/Drawable>
#include <osg/PrimitiveSet>
#include <osgViewer/ViewerEventHandlers>
#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#else
#endif

class ColorCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node ,osg::NodeVisitor* nv)
	{
		if (node)
		{
			osg::StateSet* state = node->getOrCreateStateSet();
			state->setMode(GL_BLEND, osg::StateAttribute::ON);
			
			osg::Material* pMaterial = dynamic_cast<osg::Material*>(state->getAttribute(osg::StateAttribute::MATERIAL));
			if (!pMaterial)
			{
				pMaterial = new osg::Material;
			}
			static float blend = 0.01;
			static float delta = 0.0;

			//漫射光
			pMaterial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0, 0.0, 0.0, blend));
			//设置环境光
			pMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,blend));
			//设置透明度
			pMaterial->setTransparency(osg::Material::FRONT_AND_BACK, 1-blend);
			state->setAttributeAndModes(pMaterial, osg::StateAttribute::OVERRIDE| osg::StateAttribute::ON);
			state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

			if (blend >=0.9)
			{
				delta = -0.02;
			}

			if (blend <= 0.1)
			{
				delta = 0.02;
			}
			blend += delta;
		}
	}


private:
	osgAnimation::InOutElasticFunction* m_motion;
};


osg::ref_ptr<osg::Geode> CreateGeode()
{
	osg::ref_ptr<osg::Geode> geode  = new osg::Geode;
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geode->addDrawable(geom.get());

	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	v->push_back(osg::Vec3(1.0, 1.0, 0.0));
	v->push_back(osg::Vec3(-1.0, 1.0, 0.0));
	v->push_back(osg::Vec3(-1.0, -1.0, 0.0));
	v->push_back(osg::Vec3(1.0, -1.0, 0.0));
	
	geom->setVertexArray(v.get());

	osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
	color->push_back(osg::Vec4(1.0,0.0,0.0,0.0));
	geom->setColorArray(color.get());
	geom->setColorBinding(osg::Geometry::AttributeBinding::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	//geom->getOrCreateStateSet()->setMode(GL_LIGHTING , osg::StateAttribute::OFF);
	return geode;
}

void main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	viewer->setUpViewInWindow(50,50,800,600);

	osg::ref_ptr<osg::Node> gliderModel = CreateGeode();//osgDB::readNodeFile("cow.osg");

	if (!gliderModel.valid())
	{
		std::cout << "读取模型失败" << std::endl;
		return;
	}

	gliderModel->setUpdateCallback(new ColorCallback);

	bool isSuccess = osgDB::writeNodeFile(*gliderModel.get(), "D:\\1.osg");
	viewer->setSceneData(gliderModel.get());
	viewer->addEventHandler(new osgViewer::StatsHandler);
	viewer->run();
}