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
// Created by sancar koyunlu on 5/7/13.
// Copyright (c) 2013 sancar koyunlu. All rights reserved.




#ifndef HAZELCAST_AUTHENTICATION_REQUEST
#define HAZELCAST_AUTHENTICATION_REQUEST

#include "hazelcast/client/protocol/Principal.h"
#include "hazelcast/client/protocol/UsernamePasswordCredentials.h"
#include "hazelcast/client/impl/ClientRequest.h"

namespace hazelcast {
    namespace client {
        namespace protocol {
            class AuthenticationRequest : public impl::ClientRequest {
            public:
                AuthenticationRequest(const Credentials &credential);

                void setPrincipal(Principal *principal);

                void setFirstConnection(bool);

                int getFactoryId() const;

                int getClassId() const;

                void write(serialization::PortableWriter &writer) const;

            private:
                const Credentials &credentials;
                Principal *principal;
                bool firstConnection;
            };

        }
    }
}
#endif //HAZELCAST_AUTHENTICATION_REQUEST

