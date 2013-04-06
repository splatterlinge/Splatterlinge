#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <QMainWindow>


class GLScene;
class GLView;


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0 );
	void toggleFullScreen();

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );

private:
	GLView * mGLView;
};


#endif
