//osg
#include <osgViewer/Viewer>

//sefl
#include "PickHandle.h"



PickHandle::PickHandle()
{
}

PickHandle::~PickHandle()
{
}

bool PickHandle::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	//获取viewer
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);

	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			//鼠标屏幕坐标
			float pScreenX = ea.getX();
			float pScreenY = ea.getY();

			//主相机
			osg::ref_ptr<osg::Camera> pMasterCamera = viewer->getCamera();

			//获取转换矩阵
			osg::Matrix pMVPW = pMasterCamera->getViewMatrix()*pMasterCamera->getProjectionMatrix();
			if (pMasterCamera->getViewport())
			{
				pMVPW.postMult(pMasterCamera->getViewport()->computeWindowMatrix());
			}

			osg::Matrix pInverseMVPW;
			pInverseMVPW.invert(pMVPW);

			//透视投影中视景体Znear、Zfar平面的交点
			osg::Vec3d pNearPoint = osg::Vec3d(ea.getX(), ea.getY(), 0.0)*pInverseMVPW;
			osg::Vec3d pFarPoint = osg::Vec3d(ea.getX(), ea.getY(), 1.0)*pInverseMVPW;

			//获取视点信息
			osg::Vec3 pEye, pCenter, pUp;
			pMasterCamera->getViewMatrixAsLookAt(pEye, pCenter, pUp);

			int zzz = 0;

		}

		break;
	}

	return false;
}
