#ifndef _OE_MAIN_GUI_H_
#define _OE_MAIN_GUI_H_

#include <Meta/OpenGL.h>
#include <Display/QtEnvironment.h>
#include <Utils/SimpleSetup.h>
#include <Utils/SelectionSet.h>
#include <string>
#include <QtGui>

// forward declerations
namespace Ui { class MainGUI; }
namespace OpenEngine {
namespace Display {
    class SceneNodeGUI;
    class SceneGraphGUI;
}
namespace Utils {
    class MouseSelector;
}
namespace Scene {
    class ISceneNode;
}
}

class MainGUI : public QMainWindow {
    Q_OBJECT;
    Ui::MainGUI* ui;
    OpenEngine::Display::SceneNodeGUI*                              nodeGui;
    OpenEngine::Display::SceneGraphGUI*                             graphGui;
    OpenEngine::Utils::SelectionSet<OpenEngine::Scene::ISceneNode>* ss;
    OpenEngine::Utils::MouseSelector*                               mouseSel;
public:
    MainGUI(std::string title,
            OpenEngine::Display::QtEnvironment& env,
            OpenEngine::Utils::SimpleSetup& setup);
    virtual ~MainGUI();

};

#endif // _OE_MAIN_GUI_H_
