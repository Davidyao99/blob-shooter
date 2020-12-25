#include <visualizer/shooter_app.h>

using shooter::visualizer::ShooterApp;

void prepareSettings(ShooterApp::Settings* settings) {
  settings->setResizable(false);
  settings->setConsoleWindowEnabled(true);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(ShooterApp, ci::app::RendererGl, prepareSettings);