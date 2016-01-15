/*
 * Copyright (c) 2008-2015, Hazelcast, Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// Created by İhsan Demir on 21/12/15.
//
#include <hazelcast/client/HazelcastClient.h>

int main() {
    try {
        hazelcast::client::ClientConfig config;
        hazelcast::client::HazelcastClient hz(config);

        hazelcast::client::MultiMap<std::string, std::string> map =
                hz.getMultiMap<std::string, std::string>("map");

        std::vector<std::string> keys = map.keySet();
        
        for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); ++it) {
            std::vector<std::string> values = map.get(*it);
            std::cout << *it << " -> (" ;
            for (std::vector<std::string>::const_iterator valIt = values.begin();valIt != values.end();) {
                std::cout << *valIt << std::endl;
                ++valIt;
                if (valIt != values.end()) {
                    std::cout << ", ";
                }
             }
            std::cout << ")" << std::endl;
        }
    } catch (hazelcast::client::exception::IException &e) {
        std::cerr << "Test failed !!! " << e.what() << std::endl;
        exit(-1);
    }

    std::cout << "Finished" << std::endl;

    return 0;
}