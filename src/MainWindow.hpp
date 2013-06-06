#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>
#include <QKeyEvent>


class View;


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0 );
	virtual ~MainWindow();
	void toggleFullScreen();

protected:
	// Overrides:
	void keyPressEvent( QKeyEvent * event );

private:
	View * mView;
};


#endif
