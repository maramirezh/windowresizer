#include "windowresizer/mainwindow.h"
#include "windowresizer/processlistmodel.h"
#include "ui_mainwindow.h"

#include "windowresizerutils/windowresizer.h"
#include "windowresizerutils/constants.h"
#include "windowresizerutils/windowsinterface.h"

#include <QMessageBox>

using namespace winresizer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
	processUtils(std::make_unique<ProcessUtils>(std::make_unique<WindowsInterface>()))
{
    // Setup UI, default values
    ui->setupUi(this);
    this->setWindowTitle("Window Resizer");
    ui->widthSpinbox->setRange(constants::ui::minWidth, constants::ui::maxWidth);
    ui->heightSpinbox->setRange(constants::ui::minHeight, constants::ui::maxHeight);
    ui->widthSpinbox->setValue(constants::ui::defaultWidth);
    ui->heightSpinbox->setValue(constants::ui::defaultHeight);

    ui->programSelectedLabel->setText(constants::ui::noProgramSelected);
    ui->resizeButton->setEnabled(false);

    auto* processListModel = 
        new ProcessListModel(std::make_unique<WindowsInterface>(), this);

    ui->processesTableView->setModel(processListModel);
    ui->processesTableView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->processesTableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    ui->processesTableView->resizeColumnsToContents();

    // Setup signals/slots connections
    connect(ui->actionAboutQt, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this, "About Qt");
       });
    connect(ui->actionAboutWindowResizer, &QAction::triggered, this,
        [this] {
        QMessageBox::about(this, "About WindowResizer",
            "Window Resizer is a tool designed to resize any top window of an application."
            "<br><br>"
            "Project page: "
            "<a style=\"color:white;\" href=\"https://github.com/maramirezh\">https://github.com/maramirezh</a>");
        });
    connect(ui->resizeButton, &QPushButton::clicked, this, 
        &MainWindow::onResizeButtonClicked);
    connect(ui->refreshButton, &QPushButton::clicked, processListModel,
        &ProcessListModel::resetModel);
	connect(ui->processesTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [&]() {
		const auto rowList =
			ui->processesTableView->selectionModel()->selectedRows();

		if (rowList.isEmpty()) {
			ui->programSelectedLabel->setText(constants::ui::noProgramSelected);
			ui->resizeButton->setEnabled(false);
			return;
		}
		else {
			auto index = ui->processesTableView->model()->index(
				rowList.at(0).row(),
				ProcessListModel::ProcessListColumn::ProcessName
			);

			ui->resizeButton->setEnabled(true);
			const auto programName = ui->processesTableView->model()->data(index).toString();
			ui->programSelectedLabel->setText(programName);
		}
		});
}

MainWindow::~MainWindow()
{
    delete ui;
}

uint32_t MainWindow::getSelectedItemProcessId()
{
    const auto rowList = 
        ui->processesTableView->selectionModel()->selectedRows();

    if (rowList.isEmpty()) {
        throw std::runtime_error("No items selected");
    }

    auto index = ui->processesTableView->model()->index(
        rowList.at(0).row(),
        ProcessListModel::ProcessListColumn::ProcessId
    );

    return ui->processesTableView->model()->data(index).toUInt();
}

void MainWindow::onResizeButtonClicked()
{
    try {
        const auto pid = this->getSelectedItemProcessId();
        WindowResizer windowResizer(
			std::make_unique<WindowsInterface>(),
            std::make_unique<ProcessUtils>(std::make_unique<WindowsInterface>()),
            pid
        );
        windowResizer.resizeWindow(ui->widthSpinbox->value(), ui->heightSpinbox->value());
        ui->statusbar->showMessage("Window resized!", 4000);
    }
    catch (const std::runtime_error& e) {
        QMessageBox msgBox;
        QMessageBox::warning(this, "Failed to resize window",
            QString("Could not resize window: %1").arg(e.what()));
    }
}
