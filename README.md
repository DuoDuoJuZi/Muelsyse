# Muelsyse

一个基于 ESP32 的智能家居设备固件

## 核心功能

- **Home Assistant 自动发现**: 启动后自动向 `homeassistant/light/mulsyse_led/config` 发布配置信息，无需在 HA 中手动编写 YAML。
- **MQTT 控制**: 支持通过 MQTT 主题控制板载 LED 开关。
- **断线重连**: 具备 WiFi 和 MQTT 自动重连机制。

## 硬件配置

- **主控**: ESP32 Development Board
- **执行器**: LED 连接至 GPIO 4 (高电平点亮)

## 快速上手

1. **环境准备**:
   - 安装 VS Code 和 PlatformIO 插件。
   - 准备 MQTT Broker (如 EMQX, Mosquitto) 和 Home Assistant 环境。

2. **配置修改**:
   - 复制 `secrets_example.ini` 为 `secrets.ini` 文件。
   - 修改 `secrets.ini` 文件中的以下变量：
   - `ssid`: 你的 WiFi 名称
   - `password`: 你的 WiFi 密码
   - `mqtt_server`: 你的 MQTT 服务器 IP 地址
   - `topic_cmd`: 控制 LED 开关的 MQTT 主题

3. **烧录固件**:
   - 连接 ESP32 开发板。
   - 点击 PlatformIO 侧边栏的 "Upload" 按钮。

## MQTT 接口说明

| 类型     | 主题 (Topic)                             | 消息 (Payload) | 说明                        |
| -------- | ---------------------------------------- | -------------- | --------------------------- |
| **订阅** | `home/test/led/set`                      | `ON` / `OFF`   | 控制 LED 开关               |
| **发布** | `homeassistant/light/mulsyse_led/config` | JSON Config    | Home Assistant 自动发现配置 |

## Home Assistant 集成

设备启动并连接 MQTT 后，Home Assistant 的 "MQTT" 集成下会自动发现一个名为 `LED` 的灯光实体。
确保你的 Home Assistant 已经配置并启用了 MQTT 集成。
