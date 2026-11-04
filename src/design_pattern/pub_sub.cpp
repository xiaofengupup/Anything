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


} // namespace DesginPattern

