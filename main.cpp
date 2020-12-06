#include <visualizer/shooter_app.h>

using shooter::visualizer::ShooterApp;

void prepareSettings(ShooterApp::Settings* settings) {
  settings->setResizable(false);
  settings->setConsoleWindowEnabled(true);
  settings->setFrameRate(60);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(ShooterApp, ci::app::RendererGl, prepareSettings);