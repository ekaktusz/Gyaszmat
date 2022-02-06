if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -Hsrc -Bbuild
    make -C build
fi

if [[ $1 == "clean" ]]; then
    cmake --build build --target clean
    rm -r build/*
    echo "Project cleaned."
fi


