#ifndef _AV_GRID_NODE_H_
#define _AV_GRID_NODE_H_

#include <Math/Vector.h>
#include <Scene/RenderNode.h>

using namespace OpenEngine;

class GridNode : public Scene::RenderNode {
public:

    GridNode(unsigned int number = 100,
             float        space  = 10,
             float        color  = 0);

    virtual ~GridNode();

    virtual void SetNumber(unsigned int n);
    virtual void SetSpace(float s);
    virtual void SetColor(float c);
    virtual void SetColor(Math::Vector<3,float> c);
    virtual void SetFadeColor(float c);
    virtual void SetFadeColor(Math::Vector<3,float> c);
    virtual void SetSolidColor(float c);
    virtual void SetSolidColor(Math::Vector<3,float> c);
    virtual void SetSolidRepeat(unsigned int n);

    void Apply(Renderers::IRenderingView* view);

 private:
    float width;
    float space;
    unsigned int number;
    unsigned int solidRepeat;
    Math::Vector<3,float> solidColor;
    Math::Vector<3,float> fadeColor;

};

#endif // _AV_GRID_NODE_H_
