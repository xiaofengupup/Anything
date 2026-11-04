# 项目运行
```shell
cmake -S . -B build
cmake --build build
./build/bin/Anything
```

# 设计模式
## 发布订阅模式（Publish-Subscribe Pattern）

发布订阅模式是一种典型的消息通信设计模式，用于实现模块之间的**解耦通信**。其核心思想是发布者（Publisher）不直接依赖订阅者（Subscriber），而是通过统一的消息中心（PubSubCenter）完成消息的发布与分发。

### 核心组件

- **Publisher（发布者）**：负责向指定 Topic 发布消息，无需关心消息由谁接收。
- **PubSubCenter（消息中心）**：负责维护 Topic 与 Subscriber 之间的订阅关系，并根据 Topic 将消息分发给对应的订阅者。
- **Subscriber（订阅者）**：根据业务需求订阅指定 Topic，并注册回调函数，用于处理接收到的消息。

### 核心工作流程

1. Subscriber 向 PubSubCenter 注册订阅主题及对应的回调函数。
2. PubSubCenter 保存 Topic 与回调函数之间的映射关系。
3. Publisher 向 PubSubCenter 发布指定 Topic 的消息。
4. PubSubCenter 根据 Topic 查找所有订阅者，并依次调用对应的回调函数。
5. Subscriber 通过回调函数接收并处理消息，也可以主动取消订阅。

### C++ 实现要点

- 使用 `std::function` 统一封装普通函数、Lambda 表达式及成员函数回调。
- 使用 `std::unordered_map<Topic, std::vector<Subscription>>` 管理主题与订阅关系。
- 使用唯一的 `Subscription ID` 标识订阅关系，支持精准取消订阅。
- 使用 `std::weak_ptr` 配合 Lambda 管理订阅者生命周期，避免悬空指针。

### 设计优势

- **低耦合**：发布者与订阅者无需直接依赖。
- **可扩展**：支持多个发布者和多个订阅者。
- **灵活性高**：支持动态订阅和取消订阅。
- **易维护**：通过统一消息中心管理消息通信，降低模块间的依赖关系。

该模式适用于模块间事件通知、状态同步、消息分发以及异步事件处理等场景。