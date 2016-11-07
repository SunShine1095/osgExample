#include <osgViewer/Viewer>

#include <osgDB/ReadFile>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#else
#endif

//场景模型
void AddSceneModel(osg::ref_ptr<osg::Group> pRroot)
{
	pRroot->addChild(osgDB::readNodeFile("lz.osg"));
}

//效果模型
void AddEffectModel(osg::ref_ptr<osg::Group> pRroot)
{
	//pRroot->addChild();
}

void main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;
	AddSceneModel(root);
	AddEffectModel(root);
	viewer->setSceneData(root.get());
	viewer->setUpViewInWindow(50,50,800,600);
	viewer->run();
}