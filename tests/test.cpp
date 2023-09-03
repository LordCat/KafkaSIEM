// So this I just a basic producer to test the connection to my localhost:9092 running on a WSL2 ubuntu install.
// I should hopwfully be able to use it to feed in test data as I develop the frontend and other services.
// LETTSS GOOOO!


#include <iostream>
#include <string>
#include <sys/types.h>

#include <librdkafka/rdkafkacpp.h>




int main() {

    // Lets define some repetive strings and have a place to store returned errors
    std::string topic = "test";
    std::string brokers = "localhost:9092";
    std::string errstr;
    // Create Kafka producer configuration
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    conf->set("bootstrap.servers", brokers, errstr);

    // Create Kafka producer instance
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::cerr << "Failed to create producer: " << errstr << std::endl;
        delete conf;
        return 1;
    }

    // Create a Kafka message
    RdKafka::Topic *topic_ptr = RdKafka::Topic::create(producer, topic, nullptr, errstr);
    if (!topic_ptr) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
        delete producer;
        delete conf;
        return 1;
    }

    RdKafka::ErrorCode resp;
    std::string messagePayload = "Hello, Kafka!";

    // Produce a test message
    resp = producer->produce(topic_ptr, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY, 
                             const_cast<char*>(messagePayload.c_str()), messagePayload.size(),
                             nullptr, nullptr);
    if (resp != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to produce message: " << RdKafka::err2str(resp) << std::endl;
    } else {
        std::cout << "Message produced successfully: " << messagePayload << std::endl;
    }

    // Flush and clean up resources
    producer->flush(10000);
    delete topic_ptr;
    delete producer;
    delete conf;

    return 0;
}

