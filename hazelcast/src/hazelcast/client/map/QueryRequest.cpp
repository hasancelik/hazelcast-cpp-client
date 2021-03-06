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
// Created by sancar koyunlu on 9/4/13.



#include "hazelcast/client/map/QueryRequest.h"
#include "hazelcast/client/map/PortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace map {
            QueryRequest::QueryRequest(const std::string &name, const std::string &iterationType, const std::string &sql)
            :name(name)
            , iterationType(iterationType)
            , sql(sql) {

            }

            int QueryRequest::getFactoryId() const {
                return PortableHook::F_ID;
            }

            int QueryRequest::getClassId() const {
                return PortableHook::SQL_QUERY;
            }

            void QueryRequest::write(serialization::PortableWriter &writer) const {
                writer.writeUTF("n", &name);
                writer.writeUTF("t", &iterationType);
                writer.writeUTF("sql", &sql);
            }


            bool QueryRequest::isRetryable() const {
                return true;
            }
        }
    }
}


