FROM grpc/cxx

WORKDIR /app

COPY . .

RUN g++ -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -pthread\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl \
						main.cpp
