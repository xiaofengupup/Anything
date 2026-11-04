#include "pub_sub.h"
#include <iostream>

namespace design_pattern {

bool PubSubCenter::RegisterTopic(const Topic& topic)
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);

    if (m_topics.find(topic) != m_topics.end()) {
        std::cout << "Topic " << topic << " already exists." << std::endl;
        return false;
    }
    m_topics.insert(topic);
    return true;
}

SubscriptionId PubSubCenter::Subscribe(const Topic& topic, PubSubCallback callback)
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);

    uint64_t subId = m_nextSubId++;
    m_subscribers[topic].push_back({subId, std::move(callback)});

    return subId;
}

void PubSubCenter::Unsubscribe(const Topic& topic, SubscriptionId subId)
{
    std::unique_lock<std::shared_mutex> lock(m_mutex);

    auto it = m_subscribers.find(topic);
    if (it == m_subscribers.end()) {
        std::cout << "Topic " << topic << " not found." << std::endl;
        return;
    }

    auto& subscribers = it->second;
    subscribers.erase(
        std::remove_if(subscribers.begin(), subscribers.end(), [subId](const SubscriptionNode& node) { return node.id == subId; }),
        subscribers.end()
    );
}

void PubSubCenter::Publish(const Topic& topic, const std::string& msg)
{
    std::shared_lock<std::shared_mutex> lock(m_mutex);

    auto it = m_topics.find(topic);
    if (it == m_topics.end()) {
        std::cout << "Topic " << topic << " not registered." << std::endl;
        return;
    }

    auto subscribersIt = m_subscribers.find(topic);
    if (subscribersIt == m_subscribers.end() || subscribersIt->second.empty()) {
        return;
    }

    for (auto& subscriber : subscribersIt->second) {
        subscriber.callback(msg);
    }
}

void PubSubDemo()
{
    const std::string kSpeedTopic = "/vehicle/chassis/speed";
    std::cout << "=== 智能驾驶中间件（发布者自主注册架构）模拟启动 ===" << std::endl;

    // 1、创建订阅中心
    auto pubSubCenter = std::make_shared<PubSubCenter>();

    // 发布者注册主题，这里直接调用，可在具体发布者中通过 PubSubCenter 指针调用
    pubSubCenter->RegisterTopic(kSpeedTopic);

    // 订阅者订阅主题，这里直接调用，可在具体订阅者通过 PubSubCenter 指针调用
    auto subId = pubSubCenter->Subscribe(kSpeedTopic, [](const std::string& msg) {
        std::cout << "Received speed message: " << msg << std::endl;
    });

    // 发布数据，这里直接调用，可在具体发布者中通过 PubSubCenter 指针调用
    pubSubCenter->Publish(kSpeedTopic, "10.0 m/s");

    // 取消订阅
    pubSubCenter->Unsubscribe(kSpeedTopic, subId);

    // 发布数据，此时无订阅者接收
    pubSubCenter->Publish(kSpeedTopic, "12.0 m/s");
}

} // namespace DesginPattern

