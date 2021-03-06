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
// Created by sancar koyunlu on 31/12/13.
//

#include "hazelcast/util/ByteBuffer.h"
#include "hazelcast/client/Socket.h"
#include <cassert>
#include <algorithm>
#include <string.h>

namespace hazelcast {
    namespace util {

        ByteBuffer::ByteBuffer(char *buffer, size_t capacity)
        : pos(0), lim(capacity), capacity(capacity), buffer(buffer){

        }

        ByteBuffer& ByteBuffer::flip() {
            lim = pos;
            pos = 0;
            return *this;
        }


        ByteBuffer& ByteBuffer::compact() {
            memcpy(buffer, ix(), (size_t)remaining());
            pos = remaining();
            lim = capacity;
            return *this;
        }

        ByteBuffer& ByteBuffer::clear() {
            pos = 0;
            lim = capacity;
            return *this;
        }

        size_t ByteBuffer::remaining() const {
            return lim - pos;
        }

        bool ByteBuffer::hasRemaining() const {
            return pos < lim;
        }

        size_t ByteBuffer::position() const {
            return pos;
        }


        void ByteBuffer::position(size_t i) {
            pos = i;
        }

        size_t ByteBuffer::readFrom(const client::Socket& socket, int flag) {
            size_t rm = remaining();
            size_t bytesReceived = (size_t)socket.receive(ix(), (int)rm, flag);
            safeIncrementPosition(bytesReceived);
            return bytesReceived;
        }

        void ByteBuffer::writeTo(const client::Socket& socket) {
            size_t rm = remaining();
            int bytesSend = socket.send(ix(), (int)rm);
            safeIncrementPosition((size_t)bytesSend);
        }

        int ByteBuffer::readInt() {
            char a = readByte();
            char b = readByte();
            char c = readByte();
            char d = readByte();
            return (0xff000000 & (a << 24)) |
            (0x00ff0000 & (b << 16)) |
            (0x0000ff00 & (c << 8)) |
            (0x000000ff & d);
        }

        void ByteBuffer::writeInt(int v) {
            writeByte(char(v >> 24));
            writeByte(char(v >> 16));
            writeByte(char(v >> 8));
            writeByte(char(v));
        }


        short ByteBuffer::readShort() {
            byte a = readByte();
            byte b = readByte();
            return (short)((0xff00 & (a << 8)) |
            (0x00ff & b));
        }

        void ByteBuffer::writeShort(short v) {
            writeByte(char(v >> 8));
            writeByte(char(v));
        }

        size_t ByteBuffer::writeTo(std::vector<byte>& destination, size_t offset, size_t len) {
            size_t m = std::min<size_t>(len, remaining());
            if (destination.size() < offset + m) {
                // resize the destination vector to the size of the memory needed
                destination.resize(offset + m);
            }

            memcpy((void *)(&destination[offset]), ix(), m);
            safeIncrementPosition(m);
            return m;
        }

        size_t ByteBuffer::readFrom(std::vector<byte> const& source, size_t offset, size_t len) {
            size_t minLen = std::min<size_t>(source.size() - offset, len);
            size_t m = std::min<size_t>(minLen, remaining());
            memcpy(ix(), (void *)(&source[offset]), m);
            safeIncrementPosition(m);
            return m;
        }

        byte ByteBuffer::readByte() {
            byte b = (byte)buffer[pos];
            safeIncrementPosition(1);
            return b;
        }

        void ByteBuffer::writeByte(char c) {
            buffer[pos] = c;
            safeIncrementPosition(1);
        }

        void *ByteBuffer::ix() const {
            return (void *)(buffer + pos);
        }

        void ByteBuffer::safeIncrementPosition(size_t t) {
            assert(pos + t <= capacity);
            pos += t;
        }
    }
}


