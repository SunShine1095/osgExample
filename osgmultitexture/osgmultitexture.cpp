/* OpenSceneGraph example, osgmultitexture.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Notify>

#include <osg/Texture2D>
#include <osg/TexEnv>
#include <osg/TexGen>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgUtil/Optimizer>

#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgGAd.lib")
#else
#endif


int main(int argc, char **argv)
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc, argv);

	// construct the viewer.
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50,50,800,600);
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);

	// load the nodes from the commandline arguments.
	osg::Node* rootnode = osgDB::readNodeFiles(arguments);

	// if not loaded assume no arguments passed in, try use default mode instead.
	if (!rootnode) rootnode = osgDB::readNodeFile("cessnafire.osgt");

	if (!rootnode)
	{
		osg::notify(osg::NOTICE) << "Please specify a model filename on the command line." << std::endl;
		return 1;
	}

	osg::Image* image = osgDB::readImageFile("Images/reflect.rgb");
	if (image)
	{
		//
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);

		//纹理坐标生成器
		osg::TexGen* texgen = new osg::TexGen;
		texgen->setMode(osg::TexGen::SPHERE_MAP);

		//纹理环境
		osg::TexEnv* texenv = new osg::TexEnv;
		texenv->setMode(osg::TexEnv::BLEND);
		//texenv->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 0.3f));
		texenv->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 0.0f));

		osg::StateSet* stateset = new osg::StateSet;
		stateset->setTextureAttributeAndModes(1, texture, osg::StateAttribute::ON);
		//stateset->setTextureAttributeAndModes(1, texgen, osg::StateAttribute::ON);
		//stateset->setTextureAttribute(1, texenv);

		rootnode->setStateSet(stateset);
	}
	else
	{
		osg::notify(osg::NOTICE) << "unable to load reflect map, model will not be mutlitextured" << std::endl;
	}

	// run optimization over the scene graph
	osgUtil::Optimizer optimzer;
	optimzer.optimize(rootnode);

	// add a viewport to the viewer and attach the scene graph.
	viewer.setSceneData(rootnode);

	// create the windows and run the threads.
	viewer.realize();

	for (unsigned int contextID = 0;
	contextID < osg::DisplaySettings::instance()->getMaxNumberOfGraphicsContexts();
		++contextID)
	{
		osg::GLExtensions* textExt = osg::GLExtensions::Get(contextID, false);
		if (textExt)
		{
			if (!textExt->isMultiTexturingSupported)
			{
				std::cout << "Warning: multi-texturing not supported by OpenGL drivers, unable to run application." << std::endl;
				return 1;
			}
		}
	}


	return viewer.run();
}
