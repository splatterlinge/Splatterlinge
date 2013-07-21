#ifndef STARTMENUWINDOW_HPP
#define STARTMENUWINDOW_HPP

#include <QWidget>

class Scene;
class HelpWindow;
class GfxOptionWindow;
class QPushButton;

class StartMenuWindow : public QWidget
{
	Q_OBJECT
public:
	StartMenuWindow( Scene * scene, QWidget * parent = 0 );
	~StartMenuWindow();

	GfxOptionWindow * gfxOptionWindow() { return mGfxOptionWindow; }
	HelpWindow * helpWindow() { return mHelpWindow; }

private:
	static const int WIDTH = 740;
	static const int HEIGHT = 550;
	static const int BUTTON_WIDTH = 350;
	static const int BUTTON_HEIGHT = 150;
	QPushButton * mNewGame;
	QPushButton * mOptions;
	QPushButton * mEnd;
	Scene * mScene;

	GfxOptionWindow * mGfxOptionWindow;
	HelpWindow * mHelpWindow;

	void initMenu();

private slots:
	void handleNewGameButton();
	void handleOptionsButton();
	void handleEndGameButton();

};

#endif // STARTMENUWINDOW_HPP
