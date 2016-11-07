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
	//��ȡviewer
	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);

	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::PUSH:
		if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			//�����Ļ����
			float pScreenX = ea.getX();
			float pScreenY = ea.getY();

			//�����
			osg::ref_ptr<osg::Camera> pMasterCamera = viewer->getCamera();

			//��ȡת������
			osg::Matrix pMVPW = pMasterCamera->getViewMatrix()*pMasterCamera->getProjectionMatrix();
			if (pMasterCamera->getViewport())
			{
				pMVPW.postMult(pMasterCamera->getViewport()->computeWindowMatrix());
			}

			osg::Matrix pInverseMVPW;
			pInverseMVPW.invert(pMVPW);

			//͸��ͶӰ���Ӿ���Znear��Zfarƽ��Ľ���
			osg::Vec3d pNearPoint = osg::Vec3d(ea.getX(), ea.getY(), 0.0)*pInverseMVPW;
			osg::Vec3d pFarPoint = osg::Vec3d(ea.getX(), ea.getY(), 1.0)*pInverseMVPW;

			//��ȡ�ӵ���Ϣ
			osg::Vec3 pEye, pCenter, pUp;
			pMasterCamera->getViewMatrixAsLookAt(pEye, pCenter, pUp);

			int zzz = 0;

		}

		break;
	}

	return false;
}
