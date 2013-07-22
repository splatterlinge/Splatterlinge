#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <QApplication>
#include <QMainWindow>
#include <QStyleFactory>
#include <QKeyEvent>
#include <QSettings>


class View;


/// The main window
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0 );
	virtual ~MainWindow();
	void toggleFullScreen();

protected:
	// Overrides:
	virtual void keyPressEvent( QKeyEvent * event );

private:
	View * mView;

private slots:
	void saveWindowGeometry();
};


#endif
