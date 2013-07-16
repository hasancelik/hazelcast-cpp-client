//
// Created by sancar koyunlu on 5/23/13.
// Copyright (c) 2013 hazelcast. All rights reserved.

#include "MembershipEvent.h"
#include "hazelcast/client/impl/SerializableCollection.h"
#include "hazelcast/client/protocol/AddMembershipListenerRequest.h"
#include "hazelcast/client/ClientConfig.h"
#include "hazelcast/client/spi/ClusterService.h"

namespace hazelcast {
    namespace client {
        namespace connection {
            ClusterListenerThread::ClusterListenerThread(ConnectionManager& connectionMgr, ClientConfig& clientConfig, spi::ClusterService& clusterService, spi::LifecycleService& lifecycleService, serialization::SerializationService& serializationService)
            : Thread::Thread(connection::ClusterListenerThread::run, this)
            , connectionManager(connectionMgr)
            , clientConfig(clientConfig)
            , clusterService(clusterService)
            , lifecycleService(lifecycleService)
            , serializationService(serializationService)
            , conn(NULL) {
                ;
            };

            void ClusterListenerThread::setInitialConnection(connection::Connection *connection) {
                this->conn = connection;
            };

            void *ClusterListenerThread::run(void *input) {
                static_cast<ClusterListenerThread *>(input)->runImpl();
                return NULL;

            };

            void ClusterListenerThread::runImpl() {
                while (true) {
                    try{
                        if (conn == NULL) {
                            try {
                                conn = pickConnection();
                                std::cout << "Connected: " << (*conn) << std::endl;
                            } catch (exception::IException & e) {
                                std::cout << *conn << e.what() << std::endl;
                                lifecycleService.shutdown();
                                return;
                            }
                        }
                        loadInitialMemberList();
                        listenMembershipEvents();
                    }catch(exception::IException & e){
                        if (lifecycleService.isRunning()) {
                            std::cerr << e.what() << std::endl;
                        }
                        delete conn;
                        conn = NULL;
                    }
                    try {
                        sleep(1);
                    } catch (...) {
                        break;
                    }
                }
            };

            Connection *ClusterListenerThread::pickConnection() {
                std::vector<Address> addresses;
                if (!members.empty()) {
                    vector<Address> clusterAddresses = getClusterAddresses();
                    addresses.insert(addresses.begin(), clusterAddresses.begin(), clusterAddresses.end());
                }
                vector<Address> configAddresses = getConfigAddresses();
                addresses.insert(addresses.end(), configAddresses.begin(), configAddresses.end());
                return clusterService.connectToOne(addresses);
            };

            void ClusterListenerThread::loadInitialMemberList() {
                protocol::AddMembershipListenerRequest request;
                impl::SerializableCollection coll = clusterService.sendAndReceive < impl::SerializableCollection >(conn, request);

                std::map<std::string, Member> prevMembers;
                if (!members.empty()) {
                    for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                        prevMembers[(*it).getUuid()] = *it;
                    }
                    members.clear();
                }

                const vector<serialization::Data *>& collection = coll.getCollection();
                for (vector<serialization::Data *>::const_iterator it = collection.begin(); it != collection.end(); ++it) {
                    Member member = serializationService.toObject<Member>(**it);
                    members.push_back(member);
                }
                updateMembersRef();
                std::vector<MembershipEvent> events;

                for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                    if (prevMembers.count((*it).getUuid()) > 0) {
                        prevMembers.erase((*it).getUuid());
                    } else {
                        events.push_back(MembershipEvent((*it), MembershipEvent::MEMBER_ADDED));
                    }
                }

                for (std::map< std::string, Member >::iterator it = prevMembers.begin(); it != prevMembers.end(); ++it) {
                    events.push_back(MembershipEvent(it->second, MembershipEvent::MEMBER_REMOVED));
                }


                for (std::vector<MembershipEvent>::iterator it = events.begin(); it != events.end(); ++it) {
                    clusterService.fireMembershipEvent((*it));
                }
            };

            void ClusterListenerThread::listenMembershipEvents() {
                while (true) {
                    serialization::Data data = conn->read(serializationService.getSerializationContext());
                    MembershipEvent event = serializationService.toObject<MembershipEvent>(data);
                    Member member = event.getMember();
                    if (event.getEventType() == MembershipEvent::MEMBER_ADDED) {
                        members.push_back(member);
                    } else {
                        //TODO O(n) time complexity ???
                        members.erase(std::find(members.begin(), members.end(), member));
                        connectionManager.removeConnectionPool(member.getAddress());
                    }
                    updateMembersRef();
                    clusterService.fireMembershipEvent(event);
                }
            };

            void ClusterListenerThread::updateMembersRef() {
                std::map<Address, Member> *map = new std::map<Address, Member>;
                std::cerr << "Members [" << members.size() << "]  {" << std::endl;
                for (std::vector<Member>::iterator it = members.begin(); it != members.end(); ++it) {
                    std::cerr << "\t" << (*it) << std::endl;
                    (*map)[(*it).getAddress()] = (*it);
                }
                std::cerr << "}" << std::endl;
                std::map<Address, Member> *pMap = clusterService.membersRef.set(map);
                //if (pMap != NULL)
                   // delete pMap;   //TODO should not be deleted immediately| user thread may be holding the pointer

            };

            std::vector<Address> ClusterListenerThread::getClusterAddresses() const {
                std::vector<Address> socketAddresses;
                for (std::vector<Member>::const_iterator it = members.begin(); it != members.end(); ++it) {
                    socketAddresses.push_back((*it).getAddress());
                }
                //                Collections.shuffle(socketAddresses);
                return socketAddresses;
            };

            vector<Address>  ClusterListenerThread::getConfigAddresses() const {
                std::vector<Address> socketAddresses;
                std::vector<Address>  & configAddresses = clientConfig.getAddresses();

                for (std::vector<Address>::iterator it = configAddresses.begin(); it != configAddresses.end(); ++it) {
                    socketAddresses.push_back((*it));
                }
                //                Collections.shuffle(socketAddresses);
                return socketAddresses;
            };


        }
    }
}