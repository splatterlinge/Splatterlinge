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
private:
	GLScene * glScene;
	GLView * glView;
};


#endif
