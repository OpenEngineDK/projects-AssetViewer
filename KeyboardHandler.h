#ifndef _KEYBOARD_HANDLER_
#define _KEYBOARD_HANDLER_

#include <Devices/IKeyboard.h>
#include <Display/Camera.h>
#include <Physics/FixedTimeStepPhysics.h>
#include <Physics/RigidBox.h>

using namespace OpenEngine::Devices;
using namespace OpenEngine::Physics;

class KeyboardHandler : public IModule, public IListener<KeyboardEventArg> {
private:
    bool up, down, left, right, mod;
    float step;
    Camera* camera;
    RigidBox* box;
    FixedTimeStepPhysics* physics;
public:
    KeyboardHandler(Camera* camera, RigidBox* box, FixedTimeStepPhysics* physics)
        : up(false), down(false), left(false), right(false), camera(camera), box(box), physics(physics) {}

    bool IsTypeOf(const std::type_info& inf) { return typeid(KeyboardHandler) == inf; }
    void Initialize() {
        step = 0.0f;
    }
    void Deinitialize() {}
    void Process(const float deltaTime, const float percent) {
    }

    void Handle(KeyboardEventArg arg) {
        (arg.type == KeyboardEventArg::PRESS) ? KeyDown(arg) : KeyUp(arg);
    }

    void KeyDown(KeyboardEventArg arg) {
        switch ( arg.sym ) {

        case KEY_SPACE:{
            break;
        }
        // Move the car forward
        case KEY_UP:    up    = true; break;
        case KEY_DOWN:  down  = true; break;
        case KEY_LEFT:  left  = true; break;
        case KEY_RIGHT: right = true; break;

        // Log Camera position 
        case KEY_c: {
            Vector<3,float> camPos = camera->GetPosition();
            logger.info << "Camera Position: " << camPos << logger.end;
            break;
        }

        // Quit on Escape
        case KEY_ESCAPE:
            IGameEngine::Instance().Stop();
            break;
        default: break;
        }
    }

    void KeyUp(KeyboardEventArg arg) {
        switch ( arg.sym ) {
        case KEY_UP:    up    = false; break;
        case KEY_DOWN:  down  = false; break;
        case KEY_LEFT:  left  = false; break;
        case KEY_RIGHT: right = false; break;
        case KEY_PLUS:  mod   = false; break;
        case KEY_MINUS: mod   = false; break;

        default: break;
        }
    }
    
    void BindToEventSystem() {
        IKeyboard::keyEvent.Attach(*this);
    }
};

#endif
