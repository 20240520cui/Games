# CardGame

这是一个基于 Cocos2d-x 3.17 的卡牌游戏工程。

仓库内已经预留 `cocos2d-x-3.17/` 目录；如果你把引擎源码也放进这个仓库，工程会优先使用仓库内的版本。

## 运行方式

Windows 下建议使用 CMake 生成 Visual Studio 工程，然后直接运行 `CardGame` 可执行文件。

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Debug
```

生成后的资源会自动复制到可执行文件旁边的 `Resources` 目录。

## 工程入口

- `Classes/AppDelegate.cpp`：游戏初始化和首个场景入口。
- `proj.win32/main.cpp`：Windows 启动入口。
- `proj.linux/main.cpp`：Linux 启动入口。