#ifndef DEBUGOPTIONWINDOW_INCLUDED
#define DEBUGOPTIONWINDOW_INCLUDED


#include <QWidget>


class Scene;

class QSlider;
class QCheckBox;


class DebugWindow : public QWidget
{
	Q_OBJECT

public:
	DebugWindow( Scene * scene, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~DebugWindow();

private:
	Scene * mScene;
	QCheckBox * mWireFrame;
	QCheckBox * mObjectBoundingSpheres;

public slots:
	void setWireFrame( int enable );
	void setObjectBoundingSpheres( int enable );
};


#endif
