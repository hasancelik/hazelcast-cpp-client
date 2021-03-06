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
// Created by sancar koyunlu on 18/08/14.
//

#include "hazelcast/client/multimap/MultiMapPortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"
#include "hazelcast/client/multimap/KeyBasedContainsRequest.h"

namespace hazelcast {
    namespace client {
        namespace multimap {

            KeyBasedContainsRequest::KeyBasedContainsRequest(const std::string& name, const serialization::pimpl::Data& key, const serialization::pimpl::Data& value, long threadId)
            : KeyBasedRequest(name, key)
            , hasValue(true)
            , value(value)
            , threadId(threadId) {

            }

            KeyBasedContainsRequest::KeyBasedContainsRequest(const std::string& name, const serialization::pimpl::Data& key, long threadId)
            : KeyBasedRequest(name, key)
            , hasValue(false)
            , threadId(threadId) {

            }

            int KeyBasedContainsRequest::getClassId() const {
                return MultiMapPortableHook::KEY_BASED_CONTAINS;
            }

            void KeyBasedContainsRequest::write(serialization::PortableWriter& writer) const {
                writer.writeLong("threadId", threadId);
                KeyBasedRequest::write(writer);
                serialization::ObjectDataOutput& out = writer.getRawDataOutput();
                if (hasValue) {
                    out.writeData(&value);
                } else {
                    out.writeData(NULL);
                }
            }

            bool KeyBasedContainsRequest::isRetryable() const {
                return true;
            }
        }
    }
}

