#pragma once
#include <osgGA/GUIEventHandler>
class PickHandle : public osgGA::GUIEventHandler
{
public:
	PickHandle();
	~PickHandle();

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

};

