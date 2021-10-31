#include "NanoEngine/src/NanoEngine.h"

const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

unsigned int waterBodyHeight = 0;
unsigned int waveHeight = 30;
float waveOffset = 0.0f;
float angularVelocity = -200.0f;
nano::Vec4 waveColor = nano::BLUE;

class WaveAlgorithm : public nano::Nano {
public:
    WaveAlgorithm() {

    }

    void OnCreate() override {
        nano::pixel::InitializePixels();

        waterBodyHeight = nano::pixel::PixelBufferHeight() / 2;
    }

    void Draw() {
        /*Clear the previous draw*/
        nano::pixel::Flush();

        /*Draw the water body height*/
        for (int i = 0; i < waterBodyHeight; i++) {
            for (int j = 0; j < nano::pixel::PixelBufferWidth(); j++) {
                nano::pixel::Draw(j, i, waveColor);
            }
        }

        /*Draw the sine wave*/
        float waveTop = waterBodyHeight + waveHeight;
        for (int i = waterBodyHeight; i < waveTop; i++) {
            for (int j = 0; j < nano::pixel::PixelBufferWidth(); j++) {
                float jConverted = j * 3.14159f / 100.0f;

                float output = std::sin(jConverted + waveOffset) / (jConverted < 1.0f ? jConverted + (1.0f - jConverted) : jConverted);
                float outputConverted = output * waveHeight / 1.0f;
                float outputYCoordinate = std::abs(i + outputConverted);

                if (outputYCoordinate > nano::pixel::PixelBufferHeight()) {
                    outputYCoordinate = nano::pixel::PixelBufferHeight() - 1;
                }

                nano::pixel::Draw(j, outputYCoordinate, waveColor);
            }
        }

        /*Fill in each wave*/
        auto pixels = nano::pixel::PixelBuffer();
        for (int i = waterBodyHeight; i < waveTop; i++) {
            bool isBlue = false;
            bool nonBlueConfirmed = false;
            for (int j = 0; j < nano::pixel::PixelBufferWidth(); j++) {
                if (j == 0 && (std::fabs(waveOffset) > 0.0f)) {
                    isBlue = true;
                    nonBlueConfirmed = true;
                }
                if (
                    pixels[i * 2000 + (4 * j)] / 255 == waveColor.x &&
                    pixels[i * 2000 + (4 * j) + 1] / 255 == waveColor.y &&
                    pixels[i * 2000 + (4 * j) + 2] / 255 == waveColor.z &&
                    pixels[i * 2000 + (4 * j) + 3] / 255 == waveColor.w && !isBlue) {
                    /*Set isBlue to true*/
                    isBlue = true;
                }
                else {
                    if (!(pixels[i * 2000 + (4 * j)] / 255 == waveColor.x &&
                        pixels[i * 2000 + (4 * j) + 1] / 255 == waveColor.y &&
                        pixels[i * 2000 + (4 * j) + 2] / 255 == waveColor.z &&
                        pixels[i * 2000 + (4 * j) + 3] / 255 == waveColor.w) && isBlue) {
                        /*If the color is not blue and blue was previously confirmed*/
                        nano::pixel::Draw(j, i, waveColor);
                        nonBlueConfirmed = true;
                    }
                    else {
                        /*Wave has ended*/
                        if (nonBlueConfirmed) isBlue = false;
                        nonBlueConfirmed = false;
                    }
                }
            }
        }

        /*Updates the wave offset*/
        waveOffset += ((angularVelocity * 3.14159f / 180.0f) * nano::Nano::GetDeltaTime());

        /*Wraps back around to 0.0f if the wave offset is greater than 2 pi*/
        if (waveOffset >= 2 * 3.14159f) waveOffset = 0.0f;
    }

    void OnRender() override {
        nano::Renderer renderer;

        while (nano::Nano::Running())
        {
            nano::Nano::OnInput();
            renderer.Clear();
            renderer.Background(nano::CYAN);

            Draw();

            for (int i = 0; i < nano::LayerManager::layers.size(); i++) {
                nano::Render::RenderLayer(&renderer, nano::LayerManager::layers.at(i));
            }

            nano::Nano::ManageRender();
        }
    }

    void OnInput() override {

    }
};

//int main()
//{
//    WaveAlgorithm waveAlgorithm = WaveAlgorithm();
//    waveAlgorithm.CreateApplication("Wave Algorithm", SCR_WIDTH, SCR_HEIGHT);
//    waveAlgorithm.OnCreate();
//    waveAlgorithm.OnRender();
//    waveAlgorithm.End();
//
//    return 0;
//}