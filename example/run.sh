# 功能：
# 切换路径并编译代码

# 参数：
# 编译路径
DIR="$1"

# 逻辑：
# 判断是否切换路径
if cd ${DIR}; then
    # 切换成功，开始编译
    cmake -S .. -B build -DDIR${DIR}=ON
    
    # 判断是否编译成功
    if cmake --build build; then
        # 编译成功，开始执行
        echo ">>>>"
        ./build/mcpp $@
        echo "<<<<"
    fi
fi

