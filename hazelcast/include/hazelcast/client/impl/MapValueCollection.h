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
// Created by sancar koyunlu on 6/18/13.




#ifndef HAZELCAST_MAP_VALUE_COLLECTION
#define HAZELCAST_MAP_VALUE_COLLECTION

#include "hazelcast/client/serialization/pimpl/Data.h"
#include "hazelcast/client/impl/IdentifiedDataSerializableResponse.h"
#include <vector>

namespace hazelcast {
    namespace client {
        namespace map {
            class MapValueCollection : public impl::IdentifiedDataSerializableResponse {
            public:
                MapValueCollection();

                int getFactoryId() const;

                int getClassId() const;

                void readData(serialization::ObjectDataInput &reader);

                const std::vector<serialization::pimpl::Data> &getValues() const;


            private:
                std::vector<serialization::pimpl::Data> values;
            };
        }
    }
}


#endif //HAZELCAST_MAP_VALUE_COLLECTION

