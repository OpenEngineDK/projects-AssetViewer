#ifndef _OE_MAIN_GUI_H_
#define _OE_MAIN_GUI_H_

#include <Display/QtEnvironment.h>
#include <Utils/SimpleSetup.h>
#include <string>

#include <QtGui>

// forward declerations
namespace Ui { class MainGUI; }
namespace OpenEngine {
namespace Display {
    class SceneNodeGUI;
    class SceneGraphGUI;
}
}

class MainGUI : public QMainWindow {
    Q_OBJECT;
    Ui::MainGUI* ui;
    OpenEngine::Display::SceneNodeGUI*  nodeGui;
    OpenEngine::Display::SceneGraphGUI* graphGui;

public:
    MainGUI(std::string title,
            OpenEngine::Display::QtEnvironment& env,
            OpenEngine::Utils::SimpleSetup& setup);
    virtual ~MainGUI();

};

#endif // _OE_MAIN_GUI_H_