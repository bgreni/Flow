cd build && \
cmake --build . && ./flowc ../example.flow && \
cd .. && \
llc --filetype=obj out.ll && \
gcc out.o && \
./a.out