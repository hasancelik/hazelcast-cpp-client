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
// Created by sancar koyunlu on 9/18/13.



#include "ClientTxnTest.h"
#include "HazelcastServerFactory.h"
#include "hazelcast/client/HazelcastClient.h"
#include "hazelcast/client/exception/InstanceNotActiveException.h"

namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {
            using namespace iTest;

            class MyLoadBalancer : public impl::AbstractLoadBalancer {
            public:
                const Member next() {
                    std::vector<Member> members = getMembers();
                    long len = members.size();
                    if (len == 0) {
                        throw exception::IException("const Member& RoundRobinLB::next()", "No member in member list!!");
                    }
                    for (int i = 0; i < len; i++) {
                        if (members[i].getAddress().getPort() == 5701) {
                            return members[i];
                        }
                    }
                    return members[0];
                }

            };

            class MyLifecycleListener : public MembershipListener {
            public:
                MyLifecycleListener(util::CountDownLatch& countDownLatch)
                : countDownLatch(countDownLatch) {

                }

                void memberAdded(const MembershipEvent& membershipEvent) {

                }

                void memberRemoved(const MembershipEvent& membershipEvent) {
                    countDownLatch.countDown();
                }

                void memberAttributeChanged(const MemberAttributeEvent& memberAttributeEvent) {

                }

            private:
                util::CountDownLatch& countDownLatch;
            };

            ClientTxnTest::ClientTxnTest(HazelcastServerFactory& hazelcastInstanceFactory)
            : ClientTestSupport<ClientTxnTest>("ClientTxnTest" , &hazelcastInstanceFactory)
            , hazelcastInstanceFactory(hazelcastInstanceFactory) {
            }


            ClientTxnTest::~ClientTxnTest() {
            }

            void ClientTxnTest::addTests() {
                addTest(&ClientTxnTest::testTxnRollback, "testTxnRollback");
                addTest(&ClientTxnTest::testTxnRollbackOnServerCrash, "testTxnRollbackOnServerCrash");
            }

            void ClientTxnTest::beforeClass() {
            }

            void ClientTxnTest::afterClass() {
            }

            void ClientTxnTest::beforeTest() {
                server.reset(new HazelcastServer(hazelcastInstanceFactory));
                second.reset(new HazelcastServer(hazelcastInstanceFactory));
                std::auto_ptr<ClientConfig> clientConfig = getConfig();
                clientConfig->setRedoOperation(true);
                //always start the txn on first member
                loadBalancer.reset(new MyLoadBalancer());
                clientConfig->setLoadBalancer(loadBalancer.get());
                client.reset(new HazelcastClient(*clientConfig));
            }

            void ClientTxnTest::afterTest() {
                hazelcastInstanceFactory.shutdownAll();
                client->shutdown();
                client.reset();
            }


            void ClientTxnTest::testTxnRollback() {
                util::CountDownLatch memberRemovedLatch(1);
                std::string queueName = "testTxnRollback";
                MyLifecycleListener myLifecycleListener(memberRemovedLatch);
                client->getCluster().addMembershipListener(&myLifecycleListener);

                TransactionContext context = client->newTransactionContext();
                bool rollbackSuccessful = false;
                try {
                    context.beginTransaction();
                    TransactionalQueue<std::string> queue = context.getQueue<std::string>(queueName);
                    queue.offer("item");
                    server->shutdown();
                    context.commitTransaction();
                    assertTrue(false, "commit should throw exception!!!");
                } catch (exception::IException&) {
                    context.rollbackTransaction();
                    rollbackSuccessful = true;
                }

                assertTrue(rollbackSuccessful);
                assertTrue(memberRemovedLatch.await(10), "Member removed is not signalled");

                IQueue<std::string> q = client->getQueue<std::string>(queueName);
                try {
                    assertEqual(0, q.size());
                    assertNull(q.poll().get());
                } catch (exception::IException& e) {
                    std::cout << e.what() << std::endl;
                }

            }

            void ClientTxnTest::testTxnRollbackOnServerCrash() {
                std::string queueName = "testTxnRollbackOnServerCrash";
                TransactionContext context = client->newTransactionContext();
                bool rollbackSuccessful = false;
                util::CountDownLatch memberRemovedLatch(1);
                context.beginTransaction();
                TransactionalQueue<std::string> queue = context.getQueue<std::string>(queueName);
                queue.offer("str");
                MyLifecycleListener myLifecycleListener(memberRemovedLatch);
                client->getCluster().addMembershipListener(&myLifecycleListener);
                server->shutdown();
                try {
                    context.commitTransaction();
                    assertTrue(false, "commit should throw exception!!!");
                } catch (exception::IException&) {
                    context.rollbackTransaction();
                    rollbackSuccessful = true;
                }

                assertTrue(rollbackSuccessful);
                assertTrue(memberRemovedLatch.await(10));

                IQueue<std::string> q = client->getQueue<std::string>(queueName);
                assertNull(q.poll().get());
                assertEqual(0, q.size());
            }

        }
    }
}
