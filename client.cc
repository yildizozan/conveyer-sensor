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

#include "measurement.grpc.pb.h"

using std::cout;
using std::endl;
using std::string;

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

int main(int argc, char** argv) {
  cout << "Hello, world!" << endl;
  cout << "Starting to send datas" << endl;

  string connectionString = std::getenv("GRPC_SERVER");
  
  auto channel = grpc::CreateChannel(connectionString, grpc::InsecureChannelCredentials());
  std::unique_ptr<MeasurementService::Stub> stub = MeasurementService::NewStub(channel);

  // random
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  while(true) {

    const double weight = dist(mt);
    const double humidity = dist(mt);
    const double color = dist(mt);

    Measurement m;
    m.set_weight(weight);
    m.set_humidity(humidity);
    m.set_color(color);

    OK ok;

    ClientContext context;
    Status status = stub->NewMeasurement(&context, m, &ok);
    if (!status.ok()) {
      std::cout << "RPC Failure: " << status.error_message()
        << ":" << status.error_details() << std::endl;
    }		

    printf("Weight: %f, Humidity: %f, Color: %f\n", weight, humidity, color);
    sleep(1);  
  }

  return 0;
}

