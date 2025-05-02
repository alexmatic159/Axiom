#include "Axiom.h"

#include "View/UI.h"

class DemoApp : public AXIOM::Application 
{
private:
    std::unique_ptr<UserInterface> m_UI;
	std::filesystem::path m_dbPath = AXIOM::FileManager::GetAppDataPath() / "Axiom/data/";
public:
	void Initialize() override {
		AX_INFO("Iniziallizzata Demo");

		if (!Create("Axiom Demo", 900, 650))
			AX_CRITICAL("Application creation failed");

		AXIOM::Crypt crypt;
		crypt.Decrypt(m_dbPath / "credentials.enc", m_dbPath / "credentials.json", "123");
		UserDatabase::Create(m_dbPath / "credentials.json");

        m_UI = std::make_unique<UserInterface>();
	}

	void Update(float deltaTime) override {
       
        m_UI->MainView();

	}
    
	void Shutdown() override {
		AXIOM::Crypt crypt;
		crypt.Encrypt(m_dbPath / "credentials.json", m_dbPath / "credentials.enc", "123");
		std::filesystem::remove(m_dbPath / "credentials.json");
	}

};

int main()
{
	DemoApp app;
	app.Run();
}

// TODO
// * Implementare crittografia
// * Esportare dati
// ** Implementare sistema per leggere da siti
// ** Implementare sistema per avere dati anche su telefono
// * Migliorare metodi di creazione, eliminazione e salvataggio credenziali passando indice invece di struct
// * Migliora build warning
// * Migliorare interfaccia grafica (Google password manager like)
// ** Popup eliminazione credenziale da ingrandire
// * Aggiungere icone con https://github.com/juliettef/IconFontCppHeaders.git
// * Crea un progetto di partenza di Axiom