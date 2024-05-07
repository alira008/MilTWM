def "main rr" [] {
    ./build-release/bin/MilTWM.exe
}

def "main r" [] {
    ./build-debug/bin/MilTWM.exe
}

def "main b" [] {
    cmake --build build-debug
}

def "main br" [] {
    cmake --build build-release
}

def "main rc" [] {
    cmake --preset debug -DDEBUG=ON
    cp build-debug/compile_commands.json compile_commands.json
}

def "main rcr" [] {
    cmake --preset release
    cp build-release/compile_commands.json compile_commands.json
}

def main [] {}
