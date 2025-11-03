#ifndef SCENE_CLASS_H
#define SCENE_CLASS_H


#include <random>
#include "Mesh.h"

class Scene
{
public:
	Scene();

	virtual void PreInit();
	virtual void Init();
	virtual void PostInit();

	virtual void Update(float deltaTime);
	virtual void Render();

	virtual void DebugRender();

	virtual void Shutdown();

private:

};

#endif