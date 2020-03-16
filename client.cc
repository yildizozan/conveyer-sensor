#include <unistd.h>

#include <iostream>
#include <random>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "measurement.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

int main(int argc, char** argv) {
	std::cout << "Hello, world!" << std::endl;

	auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	std::unique_ptr<MeasurementService::Stub> stub = MeasurementService::NewStub(channel);

	// random
	std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

	while(true) {

		Measurement m;
    m.set_weight(dist(mt));
    m.set_humidity(dist(mt));
    m.set_color(dist(mt));

		OK ok;

		ClientContext context;
		Status status = stub->NewMeasurement(&context, m, &ok);
		if (!status.ok()) {
      std::cout << "RPC Failure: " << status.error_message()
                << ":" << status.error_details() << std::endl;
		}		
	
		sleep(1);  
	}

	return 0;
}

