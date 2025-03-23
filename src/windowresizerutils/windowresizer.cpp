#include "windowresizerutils/windowresizer.h"
#include <utility>
#include <stdexcept>
#include "windowresizerutils/processutils.h"
#include "windowresizerutils/constants.h"
#ifdef WIN32
#include <Windows.h>
#endif

using namespace winresizer;

WindowResizer::WindowResizer(
	std::unique_ptr<OSInterface> osInterface,
	std::unique_ptr<ProcessUtils> processUtils,
	uint32_t processId) :
	processId(processId),
	osInterface(std::move(osInterface)),
	processUtils(std::move(processUtils)),
	windowHandle(this->processUtils->getTopWindowHandle(processId))
{
}

void WindowResizer::resizeWindow(int32_t width, int32_t height)
{
	const auto result = this->osInterface->SetWindowPosWrapper(
		this->windowHandle,
		nullptr,
		constants::ui::defaultPosX,
		constants::ui::defaultPosY,
		width,
		height,
		0);
	if (!result) {
		throw std::runtime_error(processUtils->getLastErrorAsString().c_str());
	}
}
