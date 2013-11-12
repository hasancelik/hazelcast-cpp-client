//
// Created by sancar koyunlu on 9/11/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_KeyBasedRequest
#define HAZELCAST_KeyBasedRequest

#include "PortableRequest.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace serialization {
            class Data;
        }
        namespace multimap {

            class KeyBasedRequest : public impl::PortableRequest {
            public:
                KeyBasedRequest(const std::string &name, const serialization::Data &key);

                virtual int getFactoryId() const;

                virtual void writePortable(serialization::PortableWriter &writer) const;

            private:
                std::string name;
                const serialization::Data &key;

            };
        }
    }
}

#endif //HAZELCAST_KeyBasedRequest