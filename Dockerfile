FROM yildizozan/grpc-cxx

WORKDIR /app

COPY . .

RUN make

CMD ["./client"]
