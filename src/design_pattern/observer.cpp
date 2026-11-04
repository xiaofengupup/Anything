#include "observer.h"

namespace design_pattern { 

void DashboardDisplay::OnStateChanged(DrivingMode new_mode)
{
    std::cout << "[DashboardDisplay] Driving mode changed to: " << ModeToString(new_mode) << std::endl;
}

void PlanningModule::OnStateChanged(DrivingMode new_mode)
{
    std::cout << "[PlanningModule] Driving mode changed to: " << ModeToString(new_mode) << std::endl;
}

void VehicleStateManager::AddObserver(std::shared_ptr<IVehicleStateObserver> observer)
{
    // 使用写锁
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_observers.push_back(observer);
}

void VehicleStateManager::SetDrivingMode(DrivingMode newMode)
{
    {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        if (m_currentMode == newMode) {
            return; // 如果模式没有变化，则不触发通知
        }

        DrivingMode oldMode = m_currentMode;
        m_currentMode = newMode;
        std::cout << "[VehicleStateManager] Driving mode set to: " << ModeToString(m_currentMode)
            << " from " << ModeToString(oldMode) << std::endl;
    }
    
    NotifyObservers(m_currentMode);
}

DrivingMode VehicleStateManager::GetCurrentMode() const
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    return m_currentMode;
}

void VehicleStateManager::NotifyObservers(DrivingMode mode)
{
    // 使用读锁即可
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    for (auto& observer : m_observers)
    {
        observer->OnStateChanged(mode);
    }
}

void ObserverDemo()
{
    std::cout << "=== 智能驾驶系统（观察者模式：状态机通知）模拟启动 ===" << std::endl;

    auto stateManager = std::make_unique<VehicleStateManager>();

    // 1. 创建仪表盘模块并注册
    auto dashboard = std::make_shared<DashboardDisplay>();
    stateManager->AddObserver(dashboard);

    // 2. 模拟系统运行在 MANUAL 模式
    stateManager->SetDrivingMode(DrivingMode::MANUAL);

    // 3. 规划模块动态加载
    auto planning = std::make_shared<PlanningModule>();
    stateManager->AddObserver(planning);

    // 切换到自动驾驶模式
    stateManager->SetDrivingMode(DrivingMode::AUTO_PILOT);

    // 模拟突发故障，切换到紧急制动
    stateManager->SetDrivingMode(DrivingMode::EMERGENCY_STOP);
}

}