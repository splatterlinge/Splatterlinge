#ifndef HELPWINDOW_INCLUDED
#define HELPWINDOW_INCLUDED


#include <QWidget>


class HelpWindow : public QWidget
{
	Q_OBJECT

public:
	HelpWindow( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	~HelpWindow();

private:

public slots:
};


#endif
