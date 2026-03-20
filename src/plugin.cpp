#include "plugin.hpp"

Plugin* pluginInstance = nullptr;

void init(Plugin* p) {
    pluginInstance = p;
    p->addModel(modelUPolM);
    p->addModel(modelUPolS);
    p->addModel(modelUMult4);
    p->addModel(modelUMult4L);
    p->addModel(modelUMult6);
    p->addModel(modelUMult6L);
    p->addModel(modelUMult8);
    p->addModel(modelUMult8L);
    p->addModel(modelUMult12);
    p->addModel(modelUMult14); p->addModel(modelUMat1604);
	p->addModel(modelUMat1601);
	p->addModel(modelUMat1601E);
}
