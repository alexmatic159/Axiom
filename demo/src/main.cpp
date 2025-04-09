#include "Axiom.h"

#include "View/UI.h"

class DemoApp : public AXIOM::Application 
{
private:
    std::unique_ptr<UserInterface> m_UI;

public:
	void Initialize() override {
		AX_INFO("Iniziallizzata Demo");

		if (!Create("Axiom Demo", 900, 650))
			AX_CRITICAL("Application creation failed");

        m_UI = std::make_unique<UserInterface>();
	}

	void Update(float deltaTime) override {
       
        m_UI->View();

	}
    
};

int main()
{
	DemoApp app;
	app.Run();
}

