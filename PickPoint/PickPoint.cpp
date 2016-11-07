//osg
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>

//self
#include "PickHandle.h"

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#else
#endif

//³¡¾°Ä£ÐÍ
void AddSceneModel(osg::ref_ptr<osg::Group> pRroot)
{
	pRroot->addChild(osgDB::readNodeFile("lz.osg"));
}

// void main()
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//	AddSceneModel(root);
//	viewer->setSceneData(root.get());
//	viewer->addEventHandler(new PickHandle);;
//	viewer->setUpViewInWindow(50, 50, 800, 600);
//	viewer->run();
//}