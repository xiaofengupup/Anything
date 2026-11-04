#pragma once

#include <cstdint>
#include <string>
#include <functional>
#include <shared_mutex>
#include <unordered_set>
#include <unordered_map>
#include <vector>

namespace design_pattern {

// 订阅主题类型
using Topic = std::string;

// 订阅关系 Id，为每一个订阅关系分配一个唯一 Id
using SubscriptionId = uint64_t;

// 统一订阅中心回调接口，屏蔽底层可调用对象的具体类型
using PubSubCallback = std::function<void(const std::string&)>;

struct SubscriptionNode {
    SubscriptionId id;       // 订阅关系 Id
    PubSubCallback callback; // 订阅回调函数
};

class PubSubCenter {
public:
    PubSubCenter() = default;
    ~PubSubCenter() = default;
    PubSubCenter(const PubSubCenter&) = delete;
    PubSubCenter& operator=(const PubSubCenter&) = delete;

public:
    bool RegisterTopic(const Topic& topic);
    SubscriptionId Subscribe(const Topic& topic, PubSubCallback callback);
    void Unsubscribe(const Topic& topic, SubscriptionId subId);
    void Publish(const Topic& topic, const std::string& msg);

private:
    std::shared_mutex m_mutex;
    uint64_t m_nextSubId = 1;
    std::unordered_set<Topic> m_topics;
    std::unordered_map<Topic, std::vector<SubscriptionNode>> m_subscribers;
};

}