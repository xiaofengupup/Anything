# 发布-订阅模式静态库
add_library(design_pattern STATIC
    pub_sub.cpp
    observer.cpp
)

# include 目录说明：
#   PUBLIC  ${CMAKE_SOURCE_DIR}/include        —— 供外部以 design_pattern/pub_sub.h 形式引用
#   PRIVATE ${CMAKE_SOURCE_DIR}/include/design_pattern —— pub_sub.cpp 内部以 pub_sub.h 形式引用
target_include_directories(design_pattern
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include/design_pattern
)

# 要求 C++17
target_compile_features(design_pattern PUBLIC cxx_std_17)
