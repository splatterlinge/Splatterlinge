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
	QLabel * farPlaneLabel;
	QSlider * farPlane;

public slots:
	void setMaterialQuality( int q );
	void setFarPlane( int distance );
};


#endif
