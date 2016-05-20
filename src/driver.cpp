//=======================================================================
// Copyright (c) 2015-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "asgard/driver.hpp"

namespace {

// Configuration
std::vector<asgard::KeyValue> config;

// The driver connection
asgard::driver_connector driver;

// The remote IDs
int source_id = -1;
int echo_action_id = -1;

void stop(){
    std::cout << "asgard:echo: stop the driver" << std::endl;

    asgard::unregister_action(driver, source_id, echo_action_id);
    asgard::unregister_source(driver, source_id);

    // Close the socket
    close(driver.socket_fd);
}

void terminate(int){
    stop();

    std::exit(0);
}

} //End of anonymous namespace

int main(){
    // Load the configuration file
    asgard::load_config(config);

    // Open the connection
    if(!asgard::open_driver_connection(driver, asgard::get_string_value(config, "server_socket_addr").c_str(), asgard::get_int_value(config, "server_socket_port"))){
        return 1;
    }

    // Register signals for "proper" shutdown
    signal(SIGTERM, terminate);
    signal(SIGINT, terminate);

     // Register the source and actions
    source_id = asgard::register_source(driver, "echo");
    echo_action_id = asgard::register_action(driver, source_id, "STRING", "echo");

    // Listen for messages from the server
    while(true){
        auto bytes_received                   = recv(driver.socket_fd, driver.receive_buffer, asgard::buffer_size, 0);
        driver.receive_buffer[bytes_received] = '\0';

        std::string message(driver.receive_buffer);
        std::stringstream message_ss(message);

        std::string command;
        message_ss >> command;

        if(command == "ACTION"){
            std::string action;
            message_ss >> action;

            if(action == "echo"){     
                std::string value;
                message_ss >> value;
                std::cout << "asgard:echo: " << value << std::endl;
            }
        }
    }

    stop();

    return 0;
}
