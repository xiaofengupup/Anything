/**
 * 观察者模式
 */
#pragma once

#include <string>
#include <iostream>
#include <shared_mutex>
#include <memory>

namespace design_pattern {

// ==================== 1. 智驾状态定义 ====================
enum class DrivingMode {
    MANUAL,         // 人工驾驶模式
    AUTO_PILOT,     // 自动驾驶模式
    EMERGENCY_STOP  // 紧急接管/停车模式
};

inline std::string ModeToString(DrivingMode mode)
{
    switch (mode) {
        case DrivingMode::MANUAL:
            return "MANUAL";
        case DrivingMode::AUTO_PILOT:
            return "AUTO_PILOT";
        case DrivingMode::EMERGENCY_STOP:
            return "EMERGENCY_STOP";
    }
    return "UNKNOWN";
}

// ==================== 2. 观察者抽象接口 ====================
class IVehicleStateObserver {
public:
    virtual ~IVehicleStateObserver() = default;

    virtual void OnStateChanged(DrivingMode new_mode) = 0;
};

// ==================== 3. 具体观察者 ====================
// 仪表显示模块
class DashboardDisplay : public IVehicleStateObserver, public std::enable_shared_from_this<DashboardDisplay> {
public:
    void OnStateChanged(DrivingMode new_mode) override;
};

// 规划决策模块
class PlanningModule : public IVehicleStateObserver, public std::enable_shared_from_this<PlanningModule> {
public:
    void OnStateChanged(DrivingMode new_mode) override;
};

// ==================== 4. 主题（Subject）：车辆状态管理器 ====================
class VehicleStateManager {
public:
    void AddObserver(std::shared_ptr<IVehicleStateObserver> observer);

    // 设置新的驾驶模式并触发通知
    void SetDrivingMode(DrivingMode newMode);

    // 获取当前驾驶模式
    DrivingMode GetCurrentMode() const;

private:
    void NotifyObservers(DrivingMode mode);

private:
    mutable std::shared_mutex m_mutex;
    uint64_t m_nextId = 0;
    DrivingMode m_currentMode = DrivingMode::MANUAL;
    std::vector<std::shared_ptr<IVehicleStateObserver>> m_observers;
};

void ObserverDemo();

} // namespace design_pattern