#include "axiom.h"

#include <iostream>

class DemoApp : public AXIOM::Application 
{
public:
	void Initialize() override {
		AX_INFO("Initializing demo");

		if (!Create("Axiom Demo", 1600, 900))
			AX_CRITICAL("Application creation failed");
	}

	void Update(float deltaTime) override {

	}
};

int main()
{
	DemoApp app;
	app.Run();
}

