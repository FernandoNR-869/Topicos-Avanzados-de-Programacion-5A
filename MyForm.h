#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

/// @namespace EvaluacionPrimerParcial
/// @brief Contenedor del proyecto para el examen del primer parcial.
namespace EvaluacionPrimerParcial {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// @class MyForm
	/// @brief Clase que controla la ventana y el procesamiento de las imágenes.
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		/// @brief Prepara la ventana y las variables de OpenCV al iniciar.
		MyForm(void)
		{
			InitializeComponent();
			camara = new cv::VideoCapture();
			frameOriginal = new cv::Mat();
			frameProcesado = new cv::Mat();
		}

	protected:
		/// @brief Limpia la memoria del programa antes de cerrarlo.
		~MyForm()
		{
			if (components) delete components;
			if (camara->isOpened()) camara->release();
			delete camara; delete frameOriginal; delete frameProcesado;
		}

	private:
		System::Windows::Forms::PictureBox^ pbVisualizador;
		System::Windows::Forms::TrackBar^ tkUmbral;
		System::Windows::Forms::Button^ btnCargar;
		System::Windows::Forms::Button^ btnCamara;
		System::Windows::Forms::ComboBox^ cbFiltros;
		System::Windows::Forms::Timer^ timerCamara;
		System::ComponentModel::IContainer^ components;

		cv::VideoCapture* camara; ///< Variable para manejar la cámara web.
		cv::Mat* frameOriginal;   ///< Guarda la imagen capturada sin cambios.
		cv::Mat* frameProcesado;  ///< Guarda el resultado después de aplicar un efecto.

		/// @brief Muestra la imagen de OpenCV en el cuadro de la ventana.
		/// @param m Imagen que se quiere mostrar.
		void ActualizarPantalla(cv::Mat m) {
			if (m.empty()) return;
			cv::Mat temp;

			if (m.channels() == 1) cv::cvtColor(m, temp, cv::COLOR_GRAY2BGRA);
			else cv::cvtColor(m, temp, cv::COLOR_BGR2BGRA);

			Bitmap^ bmpAux = gcnew Bitmap(temp.cols, temp.rows, (int)temp.step,
				System::Drawing::Imaging::PixelFormat::Format32bppPArgb, (IntPtr)temp.data);

			if (pbVisualizador->Image != nullptr) delete pbVisualizador->Image;
			pbVisualizador->Image = gcnew Bitmap(bmpAux);
		}

		/// @brief Aplica el efecto seleccionado usando el valor de la barra deslizante.
		void AplicarFiltros() {
			if (frameOriginal->empty()) return;

			int val = tkUmbral->Value;
			String^ opcion = cbFiltros->Text;

			if (opcion == "Escala de grises") {
				cv::Mat gris;
				cv::cvtColor(*frameOriginal, gris, cv::COLOR_BGR2GRAY);
				gris.convertTo(*frameProcesado, -1, 1, val - 128);
			}
			else if (opcion == "Binarización (B/N)") {
				cv::cvtColor(*frameOriginal, *frameProcesado, cv::COLOR_BGR2GRAY);
				cv::threshold(*frameProcesado, *frameProcesado, val, 255, cv::THRESH_BINARY);
			}
			else if (opcion == "Binarización (N/B)") {
				cv::cvtColor(*frameOriginal, *frameProcesado, cv::COLOR_BGR2GRAY);
				cv::threshold(*frameProcesado, *frameProcesado, val, 255, cv::THRESH_BINARY_INV);
			}
			else if (opcion == "Cany") {
				cv::Canny(*frameOriginal, *frameProcesado, val, val * 3);
			}
			else if (opcion == "Sobel") {
				cv::Mat temp, gx, gy, agx, agy;
				cv::cvtColor(*frameOriginal, temp, cv::COLOR_BGR2GRAY);
				cv::Sobel(temp, gx, CV_16S, 1, 0, 3);
				cv::Sobel(temp, gy, CV_16S, 0, 1, 3);
				cv::convertScaleAbs(gx, agx); cv::convertScaleAbs(gy, agy);
				cv::addWeighted(agx, 0.5, agy, 0.5, 0, *frameProcesado);
				cv::threshold(*frameProcesado, *frameProcesado, val, 255, cv::THRESH_TOZERO);
			}
			else if (opcion == "Separación RGB") {
				std::vector<cv::Mat> canales; cv::split(*frameOriginal, canales);
				cv::Mat vacio = cv::Mat::zeros(frameOriginal->size(), CV_8UC1);
				std::vector<cv::Mat> mezcla;
				if (val <= 85) { mezcla.push_back(canales[0]); mezcla.push_back(vacio); mezcla.push_back(vacio); }
				else if (val <= 170) { mezcla.push_back(vacio); mezcla.push_back(canales[1]); mezcla.push_back(vacio); }
				else { mezcla.push_back(vacio); mezcla.push_back(vacio); mezcla.push_back(canales[2]); }
				cv::merge(mezcla, *frameProcesado);
			}
			else if (opcion == "Rotación") {
				cv::Point2f c(frameOriginal->cols / 2.0f, frameOriginal->rows / 2.0f);
				cv::Mat r = cv::getRotationMatrix2D(c, (val * 360 / 255.0), 1.0);
				cv::warpAffine(*frameOriginal, *frameProcesado, r, frameOriginal->size());
			}
			else if (opcion == "Zoom") {
				double s = 1.0 + (val / 127.0);
				cv::Mat temp; cv::resize(*frameOriginal, temp, cv::Size(), s, s, cv::INTER_LINEAR);
				int x = (temp.cols - frameOriginal->cols) / 2;
				int y = (temp.rows - frameOriginal->rows) / 2;
				cv::Rect roi(x, y, frameOriginal->cols, frameOriginal->rows);
				*frameProcesado = temp(roi).clone();
			}
			else {
				*frameProcesado = frameOriginal->clone();
			}
			ActualizarPantalla(*frameProcesado);
		}

#pragma region Windows Form Designer
		/// @brief Configura el diseño de la ventana (colores, botones y tamaños).
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pbVisualizador = (gcnew System::Windows::Forms::PictureBox());
			this->cbFiltros = (gcnew System::Windows::Forms::ComboBox());
			this->tkUmbral = (gcnew System::Windows::Forms::TrackBar());
			this->btnCargar = (gcnew System::Windows::Forms::Button());
			this->btnCamara = (gcnew System::Windows::Forms::Button());
			this->timerCamara = (gcnew System::Windows::Forms::Timer(this->components));
			((System::ComponentModel::ISupportInitialize^)(this->pbVisualizador))->BeginInit();
			((System::ComponentModel::ISupportInitialize^)(this->tkUmbral))->BeginInit();
			this->SuspendLayout();

			this->pbVisualizador->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) | System::Windows::Forms::AnchorStyles::Left) | System::Windows::Forms::AnchorStyles::Right));
			this->pbVisualizador->Location = System::Drawing::Point(12, 46);
			this->pbVisualizador->Size = System::Drawing::Size(1200, 600);
			this->pbVisualizador->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;

			this->cbFiltros->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbFiltros->Items->AddRange(gcnew cli::array< System::Object^ >(9) {
				L"Original", L"Escala de grises", L"Binarización (B/N)", L"Binarización (N/B)",
					L"Cany", L"Sobel", L"Separación RGB", L"Rotación", L"Zoom"
			});
			this->cbFiltros->Location = System::Drawing::Point(12, 12);
			this->cbFiltros->Size = System::Drawing::Size(288, 21);
			this->cbFiltros->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::AplicarFiltros_Event);

			this->tkUmbral->Location = System::Drawing::Point(12, 660);
			this->tkUmbral->Maximum = 255;
			this->tkUmbral->Size = System::Drawing::Size(200, 45);
			this->tkUmbral->Scroll += gcnew System::EventHandler(this, &MyForm::AplicarFiltros_Event);

			this->btnCargar->Location = System::Drawing::Point(230, 660);
			this->btnCargar->Text = L"Cargar Imagen";
			this->btnCargar->Click += gcnew System::EventHandler(this, &MyForm::btnCargar_Click);

			this->btnCamara->Location = System::Drawing::Point(330, 660);
			this->btnCamara->Text = L"Abrir Cámara";
			this->btnCamara->Click += gcnew System::EventHandler(this, &MyForm::btnCamara_Click);

			this->timerCamara->Interval = 33;
			this->timerCamara->Tick += gcnew System::EventHandler(this, &MyForm::timerCamara_Tick);

			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::LightGray;
			this->ClientSize = System::Drawing::Size(1250, 750);
			this->Controls->Add(this->btnCamara);
			this->Controls->Add(this->btnCargar);
			this->Controls->Add(this->tkUmbral);
			this->Controls->Add(this->cbFiltros);
			this->Controls->Add(this->pbVisualizador);
			this->Text = L"Evaluación Primer Parcial";
			((System::ComponentModel::ISupportInitialize^)(this->pbVisualizador))->EndInit();
			((System::ComponentModel::ISupportInitialize^)(this->tkUmbral))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

	private:
		/// @brief Permite elegir una imagen guardada en la computadora.
		System::Void btnCargar_Click(System::Object^ sender, System::EventArgs^ e) {
			OpenFileDialog^ ofd = gcnew OpenFileDialog();
			ofd->Filter = "Imágenes|*.jpg;*.png;*.bmp";
			if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				timerCamara->Stop();
				std::string path = "";
				for (int i = 0; i < ofd->FileName->Length; i++) path += (char)ofd->FileName[i];
				*frameOriginal = cv::imread(path);
				AplicarFiltros();
			}
		}
		/// @brief Enciende o apaga la cámara web al presionar el botón.
		System::Void btnCamara_Click(System::Object^ sender, System::EventArgs^ e) {
			if (!timerCamara->Enabled) {
				camara->open(0);
				if (camara->isOpened()) timerCamara->Start();
			}
			else {
				timerCamara->Stop(); camara->release();
			}
		}
		/// @brief Se ejecuta cada pocos milisegundos para captar el video de la cámara.
		System::Void timerCamara_Tick(System::Object^ sender, System::EventArgs^ e) {
			camara->read(*frameOriginal);
			if (!frameOriginal->empty()) AplicarFiltros();
		}
		/// @brief Llama a la función de filtros cada vez que se mueve la barra o cambia el menú.
		System::Void AplicarFiltros_Event(System::Object^ sender, System::EventArgs^ e) {
			AplicarFiltros();
		}
	};
}