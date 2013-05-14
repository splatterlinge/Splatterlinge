#include "HelpWindow.hpp"

#include <resource/Material.hpp>
#include <scene/Scene.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QSlider>
#include <QLabel>
#include <QDebug>


HelpWindow::HelpWindow( QWidget * parent, Qt::WindowFlags f ) :
	QWidget( parent, f )
{
	setWindowTitle( tr("Help") );
	setWindowOpacity( 0.8 );

	QBoxLayout * layout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	layout->addWidget( new QLabel("F1\tThis Window\nF2\tGraphics Options") );
	setLayout( layout );

	setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );
}


HelpWindow::~HelpWindow()
{
}
