#include "AppDelegate.h"
#include "controllers/GameController.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::createWithRect("CardGame", cocos2d::Rect(0, 0, 540, 960), 1.0f);
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(540, 960, ResolutionPolicy::FIXED_WIDTH);

    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    FileUtils::getInstance()->addSearchPath("Resources");
    FileUtils::getInstance()->addSearchPath("res");

    auto gameController = GameController::create();
    gameController->startGame(1);
    gameController->retain();
    auto scene = gameController->getScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}