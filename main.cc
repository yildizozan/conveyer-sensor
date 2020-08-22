#include <unistd.h>

#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
#include <cstdio>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "position.grpc.pb.h"

using std::cout;
using std::endl;
using std::string;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using position::OK;
using position::Position;
using position::PositionService;

int main(int argc, char** argv) {
    cout << "Hello, world!" << endl;
    cout << "Starting to send datas" << endl;

    string connectionString = std::getenv("GRPC_SERVER");

    auto channel = grpc::CreateChannel(connectionString, grpc::InsecureChannelCredentials());
    std::unique_ptr<PositionService::Stub> stub = PositionService::NewStub(channel);

    // random
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.6, 1.0);

    while(true) {

        const double x = 0.0;
        const double y = 0.0;
        const double z = dist(mt);

        Position p;
        p.set_x(x);
        p.set_y(y);
        p.set_z(z);

        OK ok;

        ClientContext context;
        Status status = stub->NewPosition(&context, p, &ok);
        if (!status.ok()) {
            std::cout << "RPC Failure: " << status.error_message()
                      << ":" << status.error_details() << std::endl;
        }

        printf("x: %f, y: %f, z: %f\n", x, y, z);
        sleep(1);
    }

    return 0;
}

