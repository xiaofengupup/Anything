# 设计模式静态库
add_subdirectory(design_pattern)

# 主程序可执行文件
add_executable(Anything main.cpp)

target_link_libraries(Anything
    PRIVATE
        design_pattern
)
