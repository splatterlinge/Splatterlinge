#include "StartMenuWindow.hpp"
#include <scene/Scene.hpp>
#include "GfxOptionWindow.hpp"
#include "View.hpp"
#include<QDebug>
#include<QPushButton>
#include<QApplication>
#include<QDesktopWidget>

StartMenuWindow::StartMenuWindow(Scene * scene, QWidget *parent) :
    QWidget(parent),
    mScene( scene )
{
    setWindowTitle( tr("Start Menu") );
    setWindowOpacity( 0.8 );

    setGeometry(QRect(QPoint(10,10), QSize(WIDTH, HEIGHT)));
    activateButtons();

    setWindowFlags( Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint );
}

StartMenuWindow::~StartMenuWindow(){
}

void StartMenuWindow::activateButtons(){
    mNewGame = new QPushButton("New Game", this);

    QPalette pal = mNewGame->palette( );
    pal.setColor( QPalette::Active, QPalette::Button, Qt::darkRed );
    pal.setColor( QPalette::Inactive, QPalette::Button, Qt::darkRed );
    pal.setColor( QPalette::ButtonText, Qt::white);

    QFont buttonFont = mNewGame->font();
    buttonFont.setBold(true);
    buttonFont.setPixelSize(18);

    int xPos = width()/2.0-BUTTON_WIDTH/2.0;

    mNewGame->setGeometry(QRect(QPoint(xPos, 25), QSize(BUTTON_WIDTH, BUTTON_HEIGHT)));
    mNewGame->setPalette(pal);
    mNewGame->setFont(buttonFont);
    connect(mNewGame, SIGNAL(released()), this, SLOT(handleNewGameButton()));

    mOptions = new QPushButton("Options", this);
    mOptions ->setGeometry(QRect(QPoint(xPos, 2*25+BUTTON_HEIGHT), QSize(BUTTON_WIDTH, BUTTON_HEIGHT)));
    mOptions->setPalette(pal);
    mOptions->setFont(buttonFont);
    connect(mOptions, SIGNAL(released()), this, SLOT(handleOptionButton()));


    mEnd = new QPushButton("End Game", this);
    mEnd ->setGeometry(QRect(QPoint(xPos, 3*25+2*BUTTON_HEIGHT), QSize(BUTTON_WIDTH, BUTTON_HEIGHT)));
    mEnd->setPalette(pal);
    mEnd->setFont(buttonFont);
    connect(mEnd, SIGNAL(released()), this, SLOT(handleEndGameButton()));
}

void StartMenuWindow::handleNewGameButton(){
    qDebug() << "handleNewGameButton";
    this->setVisible(false);
}

void StartMenuWindow::handleOptionButton(){
    qDebug() << "handleOptionButton";
//    ((View*)parent())->showGfxOptionWindow(true);
}

void StartMenuWindow::handleEndGameButton(){
    exit(0);
}
