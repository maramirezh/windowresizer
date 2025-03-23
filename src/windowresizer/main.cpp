#include "windowresizer/main.h"
#include "windowresizer/mainwindow.h"
#include "windowresizerutils/constants.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <QApplication>
#include <QFile>
#include <QTextStream>

using namespace winresizer;

/**
 * @brief Initializes the logger for the application
 */
void initLogger();

/**
 * @brief Sets the application's stylesheet
 */
void setupStylesheet(QApplication& app);

int main(int argc, char** argv)
{
	//Setup logger
	initLogger();

	//Setup GUI so it doesn't look atrocious
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	setupStylesheet(app);
	
	//Show UI
	MainWindow windowResizer;
	windowResizer.show();
	return app.exec();
}

void initLogger()
{
	try {
		auto rotating_sink = std::make_shared<
			spdlog::sinks::rotating_file_sink_mt>(
			constants::logger::fileName,
			constants::logger::fileSize,
			constants::logger::maxFiles,
			constants::logger::rotateOnOpen
		);
		auto logger = std::make_shared<spdlog::logger>(
			constants::logger::loggerName, rotating_sink);
		spdlog::register_logger(logger);
		spdlog::set_default_logger(logger);
#ifndef NDEBUG
		spdlog::set_level(spdlog::level::debug);
#endif
		spdlog::info("Logger initialized successfully.");
	}
	catch (const spdlog::spdlog_ex& ex) {
		std::cerr << "Log initialization failed: " << ex.what() << '\n';
	}
}

void setupStylesheet(QApplication& app)
{
	QFile file(":/devsec.qss");
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream stream(&file);
		app.setStyleSheet(stream.readAll());
		file.close();
	}
	else {
		spdlog::warn("Could not apply stylesheet at launch");
	}
}
