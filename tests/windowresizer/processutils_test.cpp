#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "windowresizerutils/processutils.h"
#include "windowresizerutils/osinterface.h"
#include "Windows.h"

using namespace winresizer;
using namespace testing;

class MockOSInterface : public OSInterface {
public:
	MOCK_METHOD(uint32_t, GetWindowThreadProcessIdWrapper, (void*, uint32_t*), (const, override));
	MOCK_METHOD(void*, GetWindowWrapper, (void*, uint32_t), (const, override));
	MOCK_METHOD(bool, IsWindowVisibleWrapper, (void*), (const, override));
	MOCK_METHOD(void*, OpenProcessWrapper, (uint32_t, bool, uint32_t), (const, override));
    MOCK_METHOD(bool, EnumWindowsWrapper, (std::function<bool(void*, void*)>, void*), (const, override));
	MOCK_METHOD(bool, SetWindowPosWrapper, (void*, void*, int32_t, int32_t, int32_t, int32_t, uint32_t), (override));
    MOCK_METHOD(bool, EnumProcessesWrapper, (uint32_t*, uint32_t, uint32_t*), (const, override));
};

class ProcessUtilsTest : public Test {
protected:

    void SetUp() override {
        
    }

    void TearDown() override {
        
    }
};

TEST_F(ProcessUtilsTest, GIVEN_ValidProcessId_WHEN_GetProcessHandle_THEN_GetProcessHandleSucceeds) {
    auto mockOSInterface = std::make_unique<MockOSInterface>();
    void* mockHandle = reinterpret_cast<void*>(0x1234);
    uint32_t desiredAccess{};
    const bool inheritHandle{ false };
#ifdef WIN32
	desiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
#endif
    uint32_t processId = 1234;

	EXPECT_CALL(*mockOSInterface.get(), OpenProcessWrapper(
        Eq(desiredAccess), Eq(inheritHandle), Eq(processId))).
		WillRepeatedly(Return(mockHandle));

    ProcessUtils processUtils{ std::move(mockOSInterface)};

    void* handle = processUtils.getProcessHandle(processId);

    EXPECT_EQ(handle, mockHandle);
}

TEST_F(ProcessUtilsTest, GIVEN_InvalidProcessId_WHEN_GetProcessHandle_THEN_GetProcessHandleFails) {
    auto mockOSInterface = std::make_unique<MockOSInterface>();
    uint32_t desiredAccess{};
    const bool inheritHandle{ false };
#ifdef WIN32
    desiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
#endif
    uint32_t processId = 1234;

    EXPECT_CALL(*mockOSInterface.get(), OpenProcessWrapper(
        Eq(desiredAccess), Eq(inheritHandle), Eq(processId))).
        WillRepeatedly(Return(nullptr));

    ProcessUtils processUtils{ std::move(mockOSInterface) };

    try {
        void* handle = processUtils.getProcessHandle(processId);
        EXPECT_THROW(handle, std::runtime_error);
    }
    catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "Failed to get process handle");
    }
}

TEST_F(ProcessUtilsTest, GIVEN_ValidProcessId_WHEN_GetTopWindowHandle_THEN_GetTopWindowHandleSucceeds) {
    auto mockOSInterface = std::make_unique<MockOSInterface>();
    const uint32_t processId = 1234;
    void* mockHwnd = reinterpret_cast<void*>(0x1234);

    EXPECT_CALL(*mockOSInterface.get(), GetWindowThreadProcessIdWrapper(_, _))
        .WillRepeatedly(Invoke([processId](void*, uint32_t* pid) -> uint32_t {
        *pid = processId;
        return 1;
            }));

	EXPECT_CALL(*mockOSInterface.get(), GetWindowWrapper(_, _))
		.WillRepeatedly(Return(nullptr));

	EXPECT_CALL(*mockOSInterface.get(), IsWindowVisibleWrapper(_))
		.WillRepeatedly(Return(true));

    EXPECT_CALL(*mockOSInterface.get(), EnumWindowsWrapper(_, _))
        .WillRepeatedly(Invoke([mockHwnd](std::function<bool(void*, void*)> callback, void* customParam) -> bool {
        return callback(mockHwnd, customParam);
            }));

    ProcessUtils processUtils{ std::move(mockOSInterface)};

    void* hwnd = processUtils.getTopWindowHandle(processId);

    EXPECT_EQ(hwnd, mockHwnd) << "Expected: " << mockHwnd << " but was: " << hwnd;
}

TEST_F(ProcessUtilsTest, GIVEN_InvalidProcessId_WHEN_GetTopWindowHandle_THEN_GetTopWindowHandleFails) {
    auto mockOSInterface = std::make_unique<MockOSInterface>();
    const uint32_t processId = 1234;

    EXPECT_CALL(*mockOSInterface.get(), EnumWindowsWrapper(_, _))
        .WillRepeatedly(Invoke([](std::function<bool(void*, void*)> callback, void* customParam) -> bool {
        return false;
            }));

    ProcessUtils mockProcessUtils{ std::move(mockOSInterface) };

    try {
        void* hwnd = mockProcessUtils.getTopWindowHandle(processId);
		EXPECT_THROW(hwnd, std::runtime_error);
	}
	catch (const std::runtime_error& e) {
		EXPECT_STREQ(e.what(), "Could not find Top Window for given process");
	}
}

TEST_F(ProcessUtilsTest, GIVEN_ProcessesExist_WHEN_GetProcessIdList_THEN_GetProcessIdListSucceeds) {
    auto mockOSInterface = std::make_unique<MockOSInterface>();
	std::vector<uint32_t> mockProcessIdList{ 1234, 5678, 91011 };
	EXPECT_CALL(*mockOSInterface.get(), EnumProcessesWrapper(_, _, _))
		.WillRepeatedly(Invoke([&mockProcessIdList](uint32_t* aProcesses, uint32_t size, uint32_t* cbNeeded) -> bool {
        EXPECT_EQ(size, 4096);
        for (size_t i = 0; i < mockProcessIdList.size(); ++i) {
			aProcesses[i] = mockProcessIdList[i];
		}
		*cbNeeded = mockProcessIdList.size() * sizeof(uint32_t);
		return true;
			}));

    ProcessUtils mockProcessUtils{ std::move(mockOSInterface) };

    auto processIdList = mockProcessUtils.getProcessIdList();
    EXPECT_EQ(processIdList.at(0), mockProcessIdList.at(0));
    EXPECT_EQ(processIdList.at(1), mockProcessIdList.at(1));
    EXPECT_EQ(processIdList.at(2), mockProcessIdList.at(2));
}
