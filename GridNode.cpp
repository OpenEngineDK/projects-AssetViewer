#include "GridNode.h"

using namespace OpenEngine::Math;
using namespace OpenEngine::Geometry;

GridNode::GridNode(unsigned int number,
                   float space,
                   float color) {
    this->number = number;
    this->space = space;
    this->width = number * space;
    this->solidColor = Vector<3,float>(color);
    this->fadeColor = this->solidColor;
    this->solidRepeat = 1;
}

GridNode::~GridNode() {

}

void GridNode::SetNumber(unsigned int n) {
    number = n;
}

void GridNode::SetSpace(float s) {
    space = s;
}

void GridNode::SetColor(float c) {
    SetColor(Vector<3,float>(c));
}

void GridNode::SetColor(Math::Vector<3,float> c) {
    fadeColor = solidColor = c;
}

void GridNode::SetFadeColor(float c) {
    SetFadeColor(Vector<3,float>(c));
}

void GridNode::SetFadeColor(Math::Vector<3,float> c) {
    fadeColor = c;
}

void GridNode::SetSolidColor(float c) {
    SetSolidColor(Vector<3,float>(c));
}

void GridNode::SetSolidColor(Math::Vector<3,float> c) {
    solidColor = c;
}

void GridNode::SetSolidRepeat(unsigned int n) {
    solidRepeat = n;
}

void GridNode::Apply(Renderers::RenderingEventArg arg, Scene::ISceneNodeVisitor& v) {

    Renderers::IRenderer& r = arg.renderer;
    
    // draw a thicker line in origin
    float dist = 0.0;
    r.DrawLine( Line(Vector<3,float>(0.0, 0.0, -width),
                                        Vector<3,float>(0.0, 0.0,  width)), solidColor, 2);
    r.DrawLine( Line(Vector<3,float>(-width, 0.0, 0.0),
                                        Vector<3,float>( width, 0.0, 0.0)), solidColor, 2);

    // draw (number-1) lines in all directions going outwards from origin
    for (unsigned int i = 1; i < number; i++) {
        dist += space;
        Vector<3,float> c = (i % solidRepeat == 0) ? solidColor : fadeColor;
        r.DrawLine( Line(Vector<3,float>(  dist, 0.0, -width),
                                            Vector<3,float>(  dist, 0.0,  width)), c);
        r.DrawLine( Line(Vector<3,float>( -dist, 0.0, -width),
                                            Vector<3,float>( -dist, 0.0,  width)), c);
        r.DrawLine( Line(Vector<3,float>(-width, 0.0,   dist),
                                            Vector<3,float>( width, 0.0,   dist)), c);
        r.DrawLine( Line(Vector<3,float>(-width, 0.0,  -dist),
                                            Vector<3,float>( width, 0.0,  -dist)), c);
    }
}
