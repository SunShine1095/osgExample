#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Depth>
#include <osg/Camera>
#include <osgText/Text>
#include <osgGA/TrackballManipulator>
#include <osg/LineWidth>
#include <osg/Point>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <iostream>
#include <sstream>



#ifdef _DEBUG
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgTextd.lib")
#pragma comment(lib, "osgAnimationd.lib")
#else
#endif


osg::ref_ptr<osg::Group> g_grpMouse;


using namespace std;
//���������http://blog.csdn.net/tmljs1988/article/details/7814167

//�¼���
class CHUD_viewPoint : public osgGA::GUIEventHandler
{
public:
	/**���캯��*/
	CHUD_viewPoint(osgText::Text* updateText) :
		m_text(updateText) {}
	~CHUD_viewPoint() {}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	void UpdateText(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter&);

	/**LABEL*/
	void setLabel(const std::string& name)
	{
		if (m_text.get())
		{
			m_text->setText(name);
		}
	}
protected:
	osg::Vec2 m_vPosWindowMouse;//��굥�����Ĵ�������
	osg::ref_ptr<osgText::Text>  m_text;//�ӵ���Ϣ���ᶯ̬�ı�

};


bool CHUD_viewPoint::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	switch (ea.getEventType())
	{
		//case(osgGA::GUIEventAdapter::FRAME):
		//  {
		//     osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		//     if (viewer)
		//     {
		//         osg::Vec3 vCenter, vUp;
		//         viewer->getCamera()->getViewMatrixAsLookAt( m_vPosEye, vCenter, vUp);//��ȡ�ӵ���Ϣ
		//         UpdateText( viewer, ea);//����������Ϣ       
		//     }
		//     return true; 
		//  }    
	case(osgGA::GUIEventAdapter::PUSH) :
	{
		m_vPosWindowMouse.set(ea.getX(), ea.getY());//��굥�����Ĵ�������
		osgViewer::Viewer* viewer = dynamic_cast< osgViewer::Viewer*>(&aa);
		if (viewer)
		{
			UpdateText(viewer, ea);//����������Ϣ
			//�����
			osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();

			osg::Matrix mvpw = cameraMaster->getViewMatrix() * cameraMaster->getProjectionMatrix();

			if (cameraMaster->getViewport()) mvpw.postMult(cameraMaster->getViewport()->computeWindowMatrix());

			osg::Matrix _inverseMVPW;
			_inverseMVPW.invert(mvpw);

			osg::Vec3d nearPoint = osg::Vec3d(ea.getX(), ea.getY(), 0.0)* _inverseMVPW;//͸��ͶӰ��Znearƽ��Ľ���
			osg::Vec3d farPoint = osg::Vec3d(ea.getX(), ea.getY(), 1.0)* _inverseMVPW;//͸��ͶӰ��Zfarƽ��Ľ���

			osg::Vec3 vPosEye, vCenter, vUp;
			cameraMaster->getViewMatrixAsLookAt(vPosEye, vCenter, vUp);//��ȡ�ӵ���Ϣ

			osg::Matrix _inverseMV;
			_inverseMV.invert(cameraMaster->getViewMatrix());

			osg::Vec3 ptEye = osg::Vec3(0, 0, 0) * _inverseMV;//��ȡ�ӵ�����
			osg::Vec3d deltaEye = ptEye - vPosEye;

			if (deltaEye.length()< 1e-8)
			{
				cout << "yes,eye\n";
			}
			else
			{
				cout << "no,eye\n";
			}

			osg::Vec3d dir1 = farPoint - nearPoint;
			dir1.normalize();

			osg::Vec3d dir2 = farPoint - vPosEye;
			dir2.normalize();
			osg::Vec3d delta = dir1 - dir2;

			//���ӵ㡢Znearƽ��Ľ��㡢Zfarƽ��Ľ����Ƿ���ͬһֱ���ϡ�����֤��ȷ����ͬһֱ����
			if (delta.length()< 1e-8)
			{
				cout << "yes,line\n";
			}
			else
			{
				cout << "no,line\n";
			}

			osg::Geode* geode = new osg::Geode();
			osg::Geometry* pyramidGeometry = new osg::Geometry();
			geode->addDrawable(pyramidGeometry);
			osg::Vec3Array* pyramidVertices = new osg::Vec3Array;
			pyramidVertices->push_back(nearPoint);
			pyramidVertices->push_back(farPoint);
			pyramidGeometry->setVertexArray(pyramidVertices);

			//��ɫ
			osg::Vec4Array* colors = new osg::Vec4Array;
			colors->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));//��ɫ
			pyramidGeometry->setColorArray(colors);
			pyramidGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

			//����ʾ͸��ͶӰ��Znearƽ��Ľ���
			pyramidGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, 1/*3*/));
			//���߱�ʾ��������ߣ������ΪZnearƽ�潻�㣬�յ�ΪZfarƽ�潻�㡣
			pyramidGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));/**/

			osg::ref_ptr <osg::Point> ptSize = new osg::Point;
			ptSize->setSize(12.0);
			geode->getOrCreateStateSet()->setAttributeAndModes(ptSize.get(), osg::StateAttribute::ON);


			/*��ֻ��һ����ʱ����Χ��뾶Ϊ�����Կ��ܿ���������㣬����Ҫ�������ð�Χ���С���ɰѰ�Χ��뾶���㡣
			���glider��cow��Сģ�ͣ��뾶ȡ.1���ԣ���fountain.osg��.1̫С��Ϊͳһ���ɴ�Щ����*/
			osg::Vec3d ptCnt = geode->getBound().center();
			double dRadius = geode->getBound().radius();

			//�������ð�Χ��İ뾶���ɵ���setInitialBound()��
			osg::BoundingSphere bs(ptCnt, 100);

			geode->setInitialBound(bs);
			g_grpMouse->removeChildren(0, g_grpMouse->getNumChildren());
			g_grpMouse->addChild(geode);

			//��ȡ�Ӹ��ڵ㵽��ǰ�ڵ��·������
			osg::NodePathList parentNodePaths = geode->getParentalNodePaths();
			if (!parentNodePaths.empty())
			{
				osg::Matrixd mt = computeWorldToLocal(parentNodePaths[0]);
			}

			////��
			//osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
			//  osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
			//osgUtil::IntersectionVisitor iv( picker.get());
			////g_grpMouse->getParent( 0)->getChild( 0)->accept( iv);//ģ����
			//cameraMaster->accept( iv);//ģ����(��������±���)

			//��
			osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = new osgUtil::LineSegmentIntersector(
				nearPoint, farPoint);//�߶�(��ʵ����������)
			osgUtil::IntersectionVisitor iv(picker.get());
			//g_grpMouse->getParent( 0)->getChild( 0)->accept( iv);//ģ����/**/
			g_grpMouse->getParent(0)->getChild(0)->/*asGroup()->getChild( 0)->*/accept(iv);//ģ����/**/

			//               ���ڵ�     cow��MT�ڵ�
			//cameraMaster->accept( iv);//ģ����(��������±���)

			//if (picker->containsIntersections())
			//{
			//  double dLen2Shortest= DBL_MAX, dLenTmp;
			//  osgUtil::LineSegmentIntersector::Intersections::iterator hitrShortest;
			//  osgUtil::LineSegmentIntersector::Intersections intersections= picker->getIntersections();
			//  for( osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
			//     hitr != intersections.end();
			//     ++hitr)
			//  {
			//     //�����ӵ�����ĵ㣬�����ѡ�����ǰ��ĵ�
			//     dLenTmp= ( ptEye.x()- hitr->getWorldIntersectPoint().x())*
			//         ( ptEye.x()- hitr->getWorldIntersectPoint().x())+
			//         ( ptEye.y()- hitr->getWorldIntersectPoint().y())*
			//         ( ptEye.y()- hitr->getWorldIntersectPoint().y())+
			//         ( ptEye.z()- hitr->getWorldIntersectPoint().z())*
			//         ( ptEye.z()- hitr->getWorldIntersectPoint().z());
			//     if ( dLen2Shortest> dLenTmp)
			//     {
			//         dLen2Shortest= dLenTmp;
			//         hitrShortest= hitr;                   
			//     }
			//  }
			// 
			//  //���
			//  if ( dLen2Shortest != DBL_MAX)
			//  {
			//     cout<<"world coords vertex("<< hitrShortest->getWorldIntersectPoint().x()<<","
			//         << hitrShortest->getWorldIntersectPoint().y()<<","
			//         << hitrShortest->getWorldIntersectPoint().z()<<")"<<std::endl;

			//     //�����˵�              
			//     double dPointRadius= 15.0f;
			//     osg::ShapeDrawable* pShd= new osg::ShapeDrawable(
			//         new osg::Sphere( hitrShortest->getWorldIntersectPoint(), dPointRadius));//���ƽ������
			//     pShd->setColor( osg::Vec4( 0, 1, 0, 1));
			//     geode->addDrawable( pShd);
			//    
			//  }
			//}

			if (picker->containsIntersections())
			{
				osg::Vec3 ptWorldIntersectPointFirst = picker->getFirstIntersection().getWorldIntersectPoint();
				cout << "world coords vertex(" << ptWorldIntersectPointFirst.x() << ","
					<< ptWorldIntersectPointFirst.y() << "," << ptWorldIntersectPointFirst.z() << ")" << std::endl;

				//�����˵�              
				double dPointRadius = 15.0f;
				osg::ShapeDrawable* pShd = new osg::ShapeDrawable(
					new osg::Sphere(ptWorldIntersectPointFirst, dPointRadius));
				pShd->setColor(osg::Vec4(0, 1, 0, 1));
				geode->addDrawable(pShd);
			}
		}
		return true;
	}
	default:
		return false;
	}
}

void CHUD_viewPoint::UpdateText(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter&)
{
	std::string gdlist = "";
	std::ostringstream os;
	os << "MousePos(X: " << m_vPosWindowMouse.x() << ",Y: " << m_vPosWindowMouse.y() << ")";//����
	gdlist = os.str();
	setLabel(gdlist);
}

osg::Node* createHUD_viewPoint(osgText::Text* text)
{
	//��������
	std::string font("fonts/arial.TTF");//�˴����õ��Ǻ������� "fonts/STCAIYUN.TTF"
	text->setFont(font);

	//����������ʾ��λ��(����Ϊ(0,0),X�����ң�Y������)
	osg::Vec3 position(100.0f, 10.0f, 0.0f);
	text->setPosition(position);
	text->setColor(osg::Vec4(1, 1, 0, 1));
	text->setText(L"");//������ʾ������
	text->setCharacterSize(15);

	//һ��Ҫ��������Ϊ��̬���������Ῠס���������������osgcatch��
	text->setDataVariance(osg::Object::DYNAMIC);

	//������ڵ�
	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(text);//������Text����drawable���뵽Geode�ڵ���

	//����״̬
	osg::StateSet* stateset = geode->getOrCreateStateSet();

	//�رյƹ�
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//�ر���Ȳ���
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	//��GL_BLEND���ģʽ���Ա�֤Alpha������ȷ��
	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);

	//���
	osg::Camera* camera = new osg::Camera;

	//����͸�Ӿ���
	camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 600, 0, 600));//����ͶӰ   

	//���þ��Բο�����ϵ��ȷ����ͼ���󲻻ᱻ�ϼ��ڵ�ı任����Ӱ��
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	//��ͼ����ΪĬ�ϵ�
	camera->setViewMatrix(osg::Matrix::identity());


	//���ñ���Ϊ͸��������Ļ���������ClearColor 
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setAllowEventFocus(false);//����Ӧ�¼���ʼ�յò�������

	//������Ⱦ˳�򣬱����������Ⱦ
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->addChild(geode);//��Ҫ��ʾ��Geode�ڵ���뵽���
	return camera;
};


int main(int argc, char **argv)
{
	osgViewer::Viewer viewer;
	osg::ref_ptr< osg::Node> model = osgDB::readNodeFile("fountain.osg");// glider nathan

	osg::Matrix m;
	m.setTrans(200, 0, 0);
	osg::ref_ptr< osg::MatrixTransform> pmt = new osg::MatrixTransform();
	pmt->setMatrix(m);
	pmt->addChild(model.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	//root->addChild( model.get());//����ĳ��ģ��
	root->addChild(pmt.get());//����ĳ��ģ��

	osgText::Text* text = new osgText::Text;
	root->addChild(createHUD_viewPoint(text));//����HUD����
	osg::ref_ptr< CHUD_viewPoint> pHUD = new CHUD_viewPoint(text);
	viewer.addEventHandler(pHUD.get());

	//GraphicsContext�豸�����Ĺؼ�����
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
	traits->x = 200;
	traits->y = 200;
	traits->width = 600;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	//gc->setClearColor( osg::Vec4f( 0.2f,0.2f,0.2f,1.0f));
	gc->setClearColor(osg::Vec4f(0.0f, 1.0f, 0.0f, 1.0f)); //��������windows������ɫ
	gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����
	osg::ref_ptr<osg::Camera> cameraMaster = viewer.getCamera();
	cameraMaster->setGraphicsContext(gc.get());//����GraphicsContext�豸������
	//����ӿ�����
	cameraMaster->setViewport(new osg::Viewport(100, 100, traits->width, traits->height));/**/

	g_grpMouse = new osg::Group();
	//����״̬
	osg::StateSet* stateset = g_grpMouse->getOrCreateStateSet();

	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);//�رյƹ�
	//stateset->setMode( GL_DEPTH_TEST,osg::StateAttribute::OFF);//�ر���Ȳ���
	root->addChild(g_grpMouse.get());
	//viewer.setUpViewInWindow( 0, 0, 600, 600);//���ô��ڴ�С
	viewer.setSceneData(root.get());
	viewer.realize();
	viewer.run();
	return 0;
}