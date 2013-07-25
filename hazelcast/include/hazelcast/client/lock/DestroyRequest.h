//
// Created by sancar koyunlu on 6/25/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_DestroyRequest
#define HAZELCAST_DestroyRequest


#include "../serialization/SerializationConstants.h"
#include "../serialization/Data.h"
#include "LockPortableHook.h"
#include "Portable.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace lock {
            class DestroyRequest : public Portable {
            public:
            public:
                DestroyRequest(const serialization::Data& key)
                :key(key) {
                };

                int getClassId() const {
                    return LockPortableHook::GET_LOCK_COUNT;
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


#endif //HAZELCAST_DestroyRequest