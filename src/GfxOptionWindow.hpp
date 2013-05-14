#ifndef GFXOPTIONWINDOW_INCLUDED
#define GFXOPTIONWINDOW_INCLUDED


#include <QWidget>


class Scene;

class QSlider;
class QCheckBox;


class GfxOptionWindow : public QWidget
{
	Q_OBJECT

public:
	GfxOptionWindow( Scene * scene, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~GfxOptionWindow();

private:
	Scene * mScene;
	QCheckBox * wireFrame;
	QSlider * materialQuality;

public slots:
	void setMaterialQuality( int q );
	void setWireFrame( int enable );
};


#endif
