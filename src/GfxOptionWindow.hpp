#ifndef GFXOPTIONWINDOW_INCLUDED
#define GFXOPTIONWINDOW_INCLUDED


#include <QWidget>


class Scene;

class QLabel;
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
	QLabel * materialQualityLabel;
	QSlider * materialQuality;
	QLabel * materialAnisotropyLabel;
	QSlider * materialAnisotropy;
	QLabel * farPlaneLabel;
	QSlider * farPlane;
	QCheckBox * mMultiSample;

public slots:
	void setMaterialQuality( int q );
	void setMaterialFilterAnisotropy( int a );
	void setFarPlane( int distance );
	void setMultiSample( int enable );
};


#endif
