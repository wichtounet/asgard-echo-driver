//=======================================================================
// Copyright (c) 2015-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "asgard/driver.hpp"

namespace {

asgard::driver_connector driver;

void stop(){
    std::cout << "asgard:echo: stop the driver" << std::endl;

    //TODO
}

void terminate(int){
    stop();

    std::exit(0);
}

} //End of anonymous namespace

int main(){
    // Register signals for "proper" shutdown
    signal(SIGTERM, terminate);
    signal(SIGINT, terminate);

    //TODO

    stop();

    return 0;
}
