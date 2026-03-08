#include <stdexcept>

#include "common/window.h"

int main()
{
    try
    {
        USTC_CG::Window w("Hello World"); // 创建一个名为 "Hello World" 的窗口对象
        if (!w.init())
            return 1;

        w.run();
        return 0;
    }
    catch (const std::exception& e)
    {
        fprintf(stderr, "Error: %s\n", e.what());  // 输出异常信息到标准错误流
        return 1;
    }
}
/*
try 和 catch 块用于捕获和处理在程序执行过程中可能发生的异常。
在这个示例中，main 函数尝试创建一个名为 "Hello World" 的窗口，并初始化它。
如果初始化失败，程序将返回 1 表示错误。然后，程序进入窗口的主渲染循环。
*/