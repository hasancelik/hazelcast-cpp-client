//
// Created by sancar koyunlu on 6/25/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_GetRemainingLeaseRequest
#define HAZELCAST_GetRemainingLeaseRequest


#include "../serialization/SerializationConstants.h"
#include "../serialization/Data.h"
#include "LockPortableHook.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace lock {
            class GetRemainingLeaseRequest : public Portable {
            public:
                GetRemainingLeaseRequest(const serialization::Data& key)
                :key(key) {
                };

                int getClassId() const {
                    return LockPortableHook::GET_REMAINING_LEASE;
                };

                int getFactoryId() const {
                    return LockPortableHook::FACTORY_ID;
                };

                template<typename HzWriter>
                void writePortable(HzWriter& writer) const {
                    serialization::BufferedDataOutput *out = writer.getRawDataOutput();
                    key.writeData(*out);
                };

                template<typename HzReader>
                void readPortable(HzReader& reader) {
                    serialization::BufferedDataInput *in = reader.getRawDataInput();
                    key.readData(*in);
                };
            private:

                serialization::Data key;
            };
        }
    }
}


#endif //HAZELCAST_GetRemainingLeaseRequest