#pragma once

#include <QMainWindow>
#include <memory>
#include "windowresizerutils/processutils.h"

namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 * @brief The main window of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MainWindow object.
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroys the MainWindow object.
     */
    ~MainWindow();

public slots:
    /**
     * @brief Slot for handling the resize button click event.
     */
    void onResizeButtonClicked();

private:
    /**
     * @brief Gets the process ID of the selected table item.
     * @return The process ID of the selected table item.
     */
    auto getSelectedItemProcessId() -> uint32_t;

    Ui::MainWindow* ui{}; ///< The UI object for the main window.
	std::unique_ptr<winresizer::ProcessUtils> processUtils; ///< The process utility object.
};
