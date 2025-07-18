# BUPT-Tetris  
**24届BUPT计导大作业 - 俄罗斯方块控制器**

本项目是为北京邮电大学（BUPT）计算机导论课程设计的大作业，目标是实现一个**俄罗斯方块游戏控制器**，能够自动控制方块的下落与摆放逻辑，具备基本的游戏交互功能。

---


## 🛠️ 编译与运行

使用 `Makefile` 快速编译和运行项目：

```bash
# 编译项目
make

# 运行程序
make run

# 清理编译文件
make clean

# 完全清理（包括可执行文件）
make distclean

# 查看项目状态
make check
```

---

## 📁 模块说明

- **agent.c/h**：AI 控制器逻辑，实现自动放置方块的决策算法。
- **tetris.c/h**：俄罗斯方块游戏核心逻辑，包括方块生成、移动、旋转、碰撞检测等。
- **get_features.c/h**：用于从当前游戏状态中提取特征，供控制器使用。
- **utils.c/h**：通用工具函数包。
- **configurations.h**：游戏与控制器的配置常量定义。

---

## 📄 LICENSE

本项目采用 MIT License，请参见 [LICENSE](LICENSE) 文件。
