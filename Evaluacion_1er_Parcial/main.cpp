#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

/// @brief Función principal que inicia la aplicación.
/// @param args Argumentos de la línea de comandos.
/// @return Entero que indica el estado de salida del programa.
[STAThreadAttribute]
int main(array<String^>^ args) {
	// Prepara los estilos visuales de Windows para la ventana
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Crea e inicia el formulario principal del proyecto
	EvaluacionPrimerParcial::MyForm^ form = gcnew EvaluacionPrimerParcial::MyForm();
	Application::Run(form);

	return 0;
}