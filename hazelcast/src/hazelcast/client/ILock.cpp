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
// Created by sancar koyunlu on 6/27/13.



#include "hazelcast/client/ILock.h"
#include "hazelcast/client/lock/LockRequest.h"
#include "hazelcast/client/lock/UnlockRequest.h"
#include "hazelcast/client/lock/IsLockedRequest.h"
#include "hazelcast/client/lock/GetLockCountRequest.h"
#include "hazelcast/client/lock/GetRemainingLeaseRequest.h"
#include "hazelcast/util/Util.h"

namespace hazelcast {
    namespace client {
        ILock::ILock(const std::string& instanceName, spi::ClientContext *context)
        : proxy::ProxyImpl("hz:impl:lockService", instanceName, context)
        , key(toData<std::string>(instanceName)) {
            partitionId = getPartitionId(key);
        }

        void ILock::lock() {
            lock(-1);
        }

        void ILock::lock(long leaseTimeInMillis) {
            lock::LockRequest *request = new lock::LockRequest(key, util::getThreadId(), leaseTimeInMillis, -1);
            invoke(request, partitionId);
        }

        void ILock::unlock() {
            lock::UnlockRequest *request = new lock::UnlockRequest(key, util::getThreadId(), false);
            invoke(request, partitionId);
        }

        void ILock::forceUnlock() {
            lock::UnlockRequest *request = new lock::UnlockRequest(key, util::getThreadId(), true);
            invoke(request, partitionId);
        }

        bool ILock::isLocked() {
            lock::IsLockedRequest *request = new lock::IsLockedRequest(key);
            serialization::pimpl::Data data = invoke(request, partitionId);
            DESERIALIZE(data, bool);
            return *result;
        }

        bool ILock::isLockedByCurrentThread() {
            lock::IsLockedRequest *request = new lock::IsLockedRequest(key, util::getThreadId());
            serialization::pimpl::Data data = invoke(request, partitionId);
            DESERIALIZE(data, bool);
            return *result;
        }

        int ILock::getLockCount() {
            lock::GetLockCountRequest *request = new lock::GetLockCountRequest(key);
            serialization::pimpl::Data data = invoke(request, partitionId);
            DESERIALIZE(data, int);
            return *result;
        }

        long ILock::getRemainingLeaseTime() {
            lock::GetRemainingLeaseRequest *request = new lock::GetRemainingLeaseRequest(key);
            serialization::pimpl::Data data = invoke(request, partitionId);
            DESERIALIZE(data, long);
            return *result;
        }

        bool ILock::tryLock() {
            return tryLock(0);
        }

        bool ILock::tryLock(long timeInMillis) {
            lock::LockRequest *request = new lock::LockRequest(key, util::getThreadId(), -1, timeInMillis);
            serialization::pimpl::Data data = invoke(request, partitionId);
            DESERIALIZE(data, bool);
            return *result;
        }
    }
}
