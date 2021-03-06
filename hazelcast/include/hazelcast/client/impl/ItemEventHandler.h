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
// Created by sancar koyunlu on 6/24/13.





#ifndef HAZELCAST_ITEM_EVENT_HANDLER
#define HAZELCAST_ITEM_EVENT_HANDLER

#include "hazelcast/client/spi/ClusterService.h"
#include "hazelcast/client/impl/PortableItemEvent.h"
#include "hazelcast/client/ItemListener.h"
#include "hazelcast/client/ItemEvent.h"
#include "hazelcast/client/serialization/pimpl/SerializationService.h"
#include "hazelcast/client/impl/BaseEventHandler.h"

namespace hazelcast {
    namespace client {
        namespace impl {

            template<typename E>
            class ItemEventHandler : public impl::BaseEventHandler {
            public:
                ItemEventHandler(const std::string &instanceName, spi::ClusterService &clusterService, serialization::pimpl::SerializationService &serializationService, ItemListener<E> &listener, bool includeValue)
                :instanceName(instanceName)
                , clusterService(clusterService)
                , serializationService(serializationService)
                , listener(listener)
                , includeValue(includeValue) {

                };

                void handle(const client::serialization::pimpl::Data &data) {
                    boost::shared_ptr<PortableItemEvent> event = serializationService.toObject<PortableItemEvent>(data);
                    handle(*event);
                }

                void handle(const PortableItemEvent &event) {
                    boost::shared_ptr<E> item;
                    if (includeValue) {
                        item = serializationService.toObject<E>(event.getItem());
                    }
                    Member member = clusterService.getMember(event.getUuid());
                    ItemEventType type = event.getEventType();
                    ItemEvent<E> itemEvent(instanceName, type, *item, member);
                    if (type == EntryEventType::ADDED) {
                        listener.itemAdded(itemEvent);
                    } else if (type == EntryEventType::REMOVED) {
                        listener.itemRemoved(itemEvent);
                    }

                };
            private:
                const std::string &instanceName;
                spi::ClusterService &clusterService;
                serialization::pimpl::SerializationService &serializationService;
                ItemListener<E> &listener;
                bool includeValue;
            };
        }
    }
}

#endif //HAZELCAST_ITEM_EVENT_HANDLER

