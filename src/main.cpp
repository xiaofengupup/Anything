#include "design_pattern/pub_sub.h"
#include <iostream>
#include <memory>

using namespace design_pattern;

int main(int argc, char* argv[])
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