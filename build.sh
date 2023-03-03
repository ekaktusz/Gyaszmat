if [ $# -eq 0 ]; then
    cmake -DCMAKE_BUILD_TYPE=Debug -H. -Bout/build
    make -j 8 -C out/build
fi

if [[ $1 == "clean" ]]; then
    cmake --build out/build --target clean
    rm -r out/*
    echo "Project cleaned."
fi
